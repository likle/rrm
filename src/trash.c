#include "trash.h"
#include "constants.h"
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
  int min_dump;
  int max_dump;
};

static char *rrm_trash_path_join(const char *a, const char *b)
{
  size_t a_length, b_length;
  char *path;

  // First we determine the length of the two pieces which we will combine.
  // That's as simple as calling strlen.
  a_length = strlen(a);
  b_length = strlen(b);

  // Now we allocate the memory which we require to generate the new path.
  // Theoretically it would be possible to do that on the stack, but those paths
  // could be rather long. So, the length of this memory block will be the two
  // string lengths plus 2 characters (a '\0' and possibly a '/' between the two
  // strings).
  path = malloc(a_length + b_length + 2);
  if (path == NULL) {
    return NULL;
  }

  // We copy the first path to the newly allocated block.
  memcpy(path, a, a_length);

  // Check whether the last symbol of the string is a '/', if not, we will
  // append one.
  if (path[a_length - 1] != '/') {
    path[a_length] = '/';
    ++a_length;
  }

  // And now we copy the second string to our new path.
  memcpy(path + a_length, b, b_length);

  // Finally, append a '\0' to the end of the path.
  path[a_length + b_length] = '\0';

  // Return the pointer to the caller. This will have to be disposed by the
  // caller though.
  return path;
}

static int rrm_trash_create_info(const char *info_path, bool fail_if_exists)
{
  struct rrm_trash_info info;
  char *buf[255];
  int info_fd;

  info_fd = open(info_path, O_WRONLY | O_EXCL | O_CREAT);
  if (info_fd < 0) {
    if (errno == EEXIST && !fail_if_exists) {
      info_fd = open(info_path, O_WRONLY);
      if (info_fd != 0) {
        return info_fd;
      }

      return -1;
    }

    return -1;
  }

  info.dump_count = 0;
  info.min_dump = 0;
  info.max_dump = 0;

  memset(&buf, 0, sizeof(buf));
  memcpy(buf, &info, sizeof(info));

  if (pwrite(info_fd, buf, sizeof(buf), 0) != sizeof(buf)) {
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

  lock_path = rrm_trash_path_join(path, RRM_LOCK_FILE_NAME);
  if (lock_path == NULL) {
    status = RRM_SNOMEM;
    goto err_lock_path;
  }

  if ((*lock_fd = open(lock_path, O_WRONLY | O_CREAT)) < 0) {
    status = rrm_status_from_os(errno);
    goto err_open_lock;
  }

  if (lockf(*lock_fd, F_LOCK, 0) != 0) {
    status = rrm_status_from_os(errno);
    goto err_lock;
  }

  info_path = rrm_trash_path_join(path, RRM_INFO_FILE_NAME);
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

  lock_path = rrm_trash_path_join(path, RRM_LOCK_FILE_NAME);
  if (lock_path == NULL) {
    status = RRM_SNOMEM;
    goto err_lock_path;
  }

  if ((trash->lock_fd = open(lock_path, O_WRONLY)) < 0) {
    status = rrm_status_from_os(errno);
    goto err_open_lock;
  }

  if (lockf(trash->lock_fd, F_LOCK, 0) != 0) {
    status = rrm_status_from_os(errno);
    goto err_lock;
  }

  info_path = rrm_trash_path_join(path, RRM_INFO_FILE_NAME);
  if (info_path == NULL) {
    status = RRM_SNOMEM;
    goto err_info_path;
  }

  if ((trash->info_fd = open(info_path, O_WRONLY)) < 0) {
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

void rrm_trash_close(rrm_trash *trash)
{
  close(trash->lock_fd);
  close(trash->info_fd);
}
