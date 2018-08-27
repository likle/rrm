#include "dump.h"
#include "constants.h"
#include "path.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

struct dump_info
{
  int next_dump_id;
  int prev_dump_id;
  bool active;
};

static char *rrm_dump_create_path(rrm_trash *trash, int dump_id)
{
  char dump_id_buf[50];
  const char *trash_path;

  sprintf(dump_id_buf, "%i", dump_id);
  trash_path = rrm_trash_get_path(trash);
  return rrm_path_join(trash_path, dump_id_buf);
}

static rrm_status rrm_dump_create_internal(const char *path,
  bool fail_if_exists, int *lock_fd, int *info_fd)
{
  rrm_status status;
  char *lock_path, *info_path;

  if (mkdir(path, S_IRWXU) < 0) {
    if (errno == EEXIST && fail_if_exists) {
      status = RRM_SAEXST;
      return status;
    }
  }

  lock_path = rrm_path_join(path, RRM_LOCK_FILE_NAME);
  info_path = rrm_path_join(path, RRM_INFO_FILE_NAME);

  *lock_fd = open(lock_path, O_WRONLY | O_CREAT, 666);

  lockf(*lock_fd, F_LOCK, 0);

  *info_fd = open(info_path, O_WRONLY | O_CREAT, 666);

  free(info_path);
  free(lock_path);

  return RRM_SOK;
}

rrm_status rrm_dump_create(rrm_trash *trash, int dump_id)
{
  rrm_status status;
  char *dump_path;
  int lock_fd, info_fd;

  dump_path = rrm_dump_create_path(trash, dump_id);
  status = rrm_dump_create_internal(dump_path, true, &lock_fd, &info_fd);
  if (rrm_status_is_error(status)) {
    return status;
  }

  close(info_fd);
  lockf(lock_fd, F_ULOCK, 0);
  close(lock_fd);

  return RRM_SOK;
}

rrm_status rrm_dump_open(rrm_dump *dump, rrm_trash *trash, int dump_id,
  bool create_if_missing)
{
  char *lock_path, *info_path;

  dump->path = rrm_dump_create_path(trash, dump_id);
  if (create_if_missing) {
    return rrm_dump_create_internal(dump->path, false, &dump->lock_fd,
      &dump->info_fd);
  }

  lock_path = rrm_path_join(dump->path, RRM_LOCK_FILE_NAME);
  info_path = rrm_path_join(dump->path, RRM_INFO_FILE_NAME);

  dump->lock_fd = open(lock_path, O_WRONLY);
  lockf(dump->lock_fd, F_LOCK, 0);

  dump->info_fd = open(info_path, O_WRONLY);

  return RRM_SOK;
}

rrm_status rrm_dump_configure(rrm_dump *dump, int next_dump_id,
  int prev_dump_id, bool active)
{
  struct dump_info info;
  char dump_id_buf[50];
  char dump_buf[sizeof(struct dump_info)];

  info.next_dump_id = next_dump_id;
  info.prev_dump_id = prev_dump_id;
  info.active = active;

  memcpy(dump_buf, &info, sizeof(dump_buf));
  pwrite(dump->info_fd, dump_id_buf, sizeof(dump_buf), 0);

  return RRM_SOK;
}

void rrm_dump_close(rrm_dump *dump)
{
  close(dump->info_fd);
  lockf(dump->lock_fd, F_ULOCK, 0);
  close(dump->lock_fd);
}
