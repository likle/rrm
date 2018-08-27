#include "dump.h"
#include "constants.h"
#include "path.h"
#include "trash.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

struct rrm_dump_info
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
  struct rrm_dump_info info;

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
  info.prev_dump_id = 0;
  info.next_dump_id = 0;
  info.active = true;
  pwrite(*info_fd, &info, sizeof(info), 0);

  free(info_path);
  free(lock_path);

  return RRM_SOK;
}

rrm_status rrm_dump_create(struct rrm_trash *trash, int dump_id)
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

rrm_status rrm_dump_open(rrm_dump *dump, struct rrm_trash *trash, int dump_id,
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

  dump->dump_id = dump_id;

  dump->lock_fd = open(lock_path, O_WRONLY);
  lockf(dump->lock_fd, F_LOCK, 0);

  dump->info_fd = open(info_path, O_WRONLY);

  return RRM_SOK;
}

rrm_status rrm_dump_configure(rrm_dump *dump, struct rrm_dump_info info)
{
  pwrite(dump->info_fd, &info, sizeof(info), 0);

  return RRM_SOK;
}

rrm_status rrm_dump_move_after(rrm_dump *dump, rrm_dump *new_dump)
{
  struct rrm_dump_info info, new_info;

  pread(dump->info_fd, &info, sizeof(info), 0);
  pread(new_dump->info_fd, &new_info, sizeof(new_info), 0);

  assert(new_info.prev_dump_id == 0);

  info.next_dump_id = new_dump->dump_id;
  new_info.prev_dump_id = dump->dump_id;

  pwrite(dump->info_fd, &info, sizeof(info), 0);
  pwrite(dump->info_fd, &new_info, sizeof(new_info), 0);

  return RRM_SOK;
}

rrm_status rrm_dump_next(rrm_dump *dump)
{
  rrm_status status;
  rrm_dump next;
  struct rrm_dump_info info;

  if (pread(dump->info_fd, &info, sizeof(info), 0) != sizeof(info)) {
    return rrm_status_from_os(errno);
  }

  if (info.next_dump_id == 0) {
    return RRM_SEND;
  }

  status = rrm_dump_open(&next, dump->trash, info.next_dump_id, false);
  if (rrm_status_is_error(status)) {
    return status;
  }

  rrm_dump_close(dump);

  *dump = next;

  return RRM_SOK;
}

rrm_status rrm_dump_previous(rrm_dump *dump)
{
  rrm_status status;
  rrm_dump prev;
  struct rrm_dump_info info;

  if (pread(dump->info_fd, &info, sizeof(info), 0) != sizeof(info)) {
    return rrm_status_from_os(errno);
  }

  if (info.next_dump_id == 0) {
    return RRM_SEND;
  }

  rrm_dump_close(dump);

  status = rrm_dump_open(&prev, dump->trash, info.prev_dump_id, false);
  if (rrm_status_is_error(status)) {
    // TODO iterator invalid
    return status;
  }

  // TODO move right

  *dump = prev;

  return RRM_SOK;
}

void rrm_dump_close(rrm_dump *dump)
{
  close(dump->info_fd);
  lockf(dump->lock_fd, F_ULOCK, 0);
  close(dump->lock_fd);
}
