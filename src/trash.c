#include "trash.h"
#include "constants.h"
#include "dump.h"
#include "path.h"
#include "status.h"
#include <assert.h>
#include <errno.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * This is the trash info which we store on disk. It can contain basic
 * information about the amount of dumps and it could contain statistics as
 * well. This has to be initialized when the trash is created, and updated when
 * a dump is added or removed.
 */
struct rrm_trash_info
{
  int dump_count;
  int free_dump;
  int first_dump;
  int last_dump;
};

static int rrm_trash_write_info(int info_fd, struct rrm_trash_info *info)
{
  if (pwrite(info_fd, info, sizeof(*info), 0) != sizeof(*info)) {
    return -1;
  }

  return 0;
}

static int rrm_trash_read_info(int info_fd, struct rrm_trash_info *info)
{
  if (pread(info_fd, info, sizeof(*info), 0) != sizeof(*info)) {
    printf("%i, %s", errno, strerror(errno));
    return -1;
  }

  return 0;
}

static int rrm_trash_create_info(const char *info_path, bool fail_if_exists)
{
  struct rrm_trash_info info;
  int info_fd;

  info_fd = open(info_path, O_RDWR | O_EXCL | O_CREAT, 0666);
  if (info_fd < 0) {
    if (errno == EEXIST && !fail_if_exists) {
      info_fd = open(info_path, O_RDWR);
      if (info_fd != 0) {
        return info_fd;
      }

      return -1;
    }

    return -1;
  }

  info.dump_count = 0;
  info.free_dump = 0;
  info.first_dump = 0;
  info.last_dump = 0;

  if (rrm_trash_write_info(info_fd, &info) < 0) {
    close(info_fd);
    return -1;
  }

  return info_fd;
}

rrm_status rrm_trash_create_internal(const char *path, bool fail_if_exists,
  int *lock_fd, int *info_fd)
{
  rrm_status status;
  char *lock_path, *info_path;

  if (mkdir(path, S_IRWXU) != 0) {
    if (errno != EEXIST) {
      status = rrm_status_from_os(errno);
      goto err_mkdir;
    }
  }

  lock_path = rrm_path_join(path, RRM_LOCK_FILE_NAME);
  if (lock_path == NULL) {
    status = RRM_SNOMEM;
    goto err_lock_path;
  }

  if ((*lock_fd = open(lock_path, O_RDWR | O_CREAT, 0666)) < 0) {
    status = rrm_status_from_os(errno);
    goto err_open_lock;
  }

  if (lockf(*lock_fd, F_LOCK, 0) != 0) {
    status = rrm_status_from_os(errno);
    goto err_lock;
  }

  info_path = rrm_path_join(path, RRM_INFO_FILE_NAME);
  if (info_path == NULL) {
    status = RRM_SNOMEM;
    goto err_info_path;
  }

  if ((*info_fd = rrm_trash_create_info(info_path, fail_if_exists)) == -1) {
    status = rrm_status_from_os(errno);
    goto err_info;
  }

  lockf(*lock_fd, F_ULOCK, 0);

  free(lock_path);
  free(info_path);

  return RRM_SOK;

err_info:
  free(info_path);
err_info_path:
  lockf(*lock_fd, F_ULOCK, 0);
err_lock:
  close(*lock_fd);
err_open_lock:
  free(lock_path);
err_lock_path:
err_mkdir:
  return status;
}

rrm_status rrm_trash_create(const char *path)
{
  rrm_status status;
  int lock_fd, info_fd;

  status = rrm_trash_create_internal(path, true, &lock_fd, &info_fd);
  if (rrm_status_is_error(status)) {
    return status;
  }

  close(info_fd);
  close(lock_fd);

  return RRM_SOK;
}

rrm_status rrm_trash_open(rrm_trash *trash, const char *path,
  bool create_if_missing)
{
  rrm_status status;
  char *lock_path, *info_path;

  trash->path = path;

  if (create_if_missing) {
    return rrm_trash_create_internal(path, false, &trash->lock_fd,
      &trash->info_fd);
  }

  lock_path = rrm_path_join(path, RRM_LOCK_FILE_NAME);
  if (lock_path == NULL) {
    status = RRM_SNOMEM;
    goto err_lock_path;
  }

  if ((trash->lock_fd = open(lock_path, O_RDWR)) < 0) {
    status = rrm_status_from_os(errno);
    goto err_open_lock;
  }

  if (lockf(trash->lock_fd, F_LOCK, 0) != 0) {
    status = rrm_status_from_os(errno);
    goto err_lock;
  }

  info_path = rrm_path_join(path, RRM_INFO_FILE_NAME);
  if (info_path == NULL) {
    status = RRM_SNOMEM;
    goto err_info_path;
  }

  if ((trash->info_fd = open(info_path, O_RDWR)) < 0) {
    status = rrm_status_from_os(errno);
    goto err_open_info;
  }

  free(info_path);
  lockf(trash->lock_fd, F_ULOCK, 0);
  free(lock_path);

  return RRM_SOK;

err_open_info:
  free(info_path);
err_info_path:
  lockf(trash->lock_fd, F_ULOCK, 0);
err_lock:
  close(trash->lock_fd);
err_open_lock:
  free(lock_path);
err_lock_path:
  return status;
}

const char *rrm_trash_get_path(rrm_trash *trash)
{
  return trash->path;
}

static rrm_status rrm_trash_insert_at_end(rrm_trash *trash, int last_dump_id,
  int dump_id, rrm_dump *dump)
{
  rrm_dump last_dump;
  rrm_dump_open(&last_dump, trash, last_dump_id, false);
  rrm_dump_open(dump, trash, dump_id, true);
  rrm_dump_move_after(&last_dump, dump);
  rrm_dump_close(&last_dump);

  return RRM_SOK;
}

static rrm_status rrm_trash_allocate_dump_id(rrm_trash *trash, int *dump_id,
  int *last_dump_id)
{
  struct rrm_trash_info info;

  lockf(trash->lock_fd, F_LOCK, 0);
  rrm_trash_read_info(trash->info_fd, &info);
  *last_dump_id = info.last_dump;
  info.last_dump = *dump_id = ++info.dump_count;
  rrm_trash_write_info(trash->info_fd, &info);
  lockf(trash->lock_fd, F_ULOCK, 0);
  return RRM_SOK;
}

rrm_status rrm_trash_insert(rrm_trash *trash, rrm_dump *dump)
{
  rrm_status status;
  int dump_id, last_dump_id;

  status = rrm_trash_allocate_dump_id(trash, &dump_id, &last_dump_id);
  if (rrm_status_is_error(status)) {
    goto err_allocate_dump_id;
  }

  if (last_dump_id != 0) {
    status = rrm_trash_insert_at_end(trash, last_dump_id, dump_id, dump);
    if (rrm_status_is_error(status)) {
      goto err_insert_at_end;
    }

    return RRM_SOK;
  }

  return rrm_dump_open(dump, trash, dump_id, true);

err_insert_at_end:
  // TODO undo dump id allocation
err_allocate_dump_id:
  return status;
}

void rrm_trash_close(rrm_trash *trash)
{
  close(trash->lock_fd);
  close(trash->info_fd);
}
