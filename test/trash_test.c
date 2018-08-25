#include <constants.h>
#include <stdio.h>
#include <stdlib.h>
#include <trash.h>
#include <unistd.h>

static int clear()
{
  remove("my_trash/" RRM_LOCK_FILE_NAME);
  remove("my_trash/" RRM_INFO_FILE_NAME);
  remove("my_trash");

  if (access("my_trash", F_OK) == 0) {
    return -1;
  }

  return 0;
}

int trash_open_fail_no_entry()
{
  rrm_status status;
  rrm_trash trash;

  if (clear() != 0) {
    return EXIT_FAILURE;
  }

  status = rrm_trash_open(&trash, "my_trash", false);
  if (!rrm_status_is_error(status)) {
    return EXIT_FAILURE;
  }

  clear();

  return EXIT_SUCCESS;
}

int trash_create_fail_exists()
{
  rrm_status status;

  if (clear() != 0) {
    return EXIT_FAILURE;
  }

  status = rrm_trash_create("my_trash");
  if (rrm_status_is_error(status)) {
    return EXIT_FAILURE;
  }

  if (access("my_trash", F_OK) != 0) {
    return EXIT_FAILURE;
  }

  status = rrm_trash_create("my_trash");
  if (!rrm_status_is_error(status)) {
    return EXIT_FAILURE;
  }

  clear();

  return EXIT_SUCCESS;
}

int trash_create_open()
{
  rrm_status status;
  rrm_trash trash;

  if (clear() != 0) {
    return EXIT_FAILURE;
  }

  status = rrm_trash_create("my_trash");
  if (rrm_status_is_error(status)) {
    return EXIT_FAILURE;
  }

  status = rrm_trash_open(&trash, "my_trash", false);
  if (rrm_status_is_error(status)) {
    return EXIT_FAILURE;
  }

  if (access("my_trash", F_OK) != 0) {
    return EXIT_FAILURE;
  }

  clear();

  return EXIT_SUCCESS;
}

int trash_open_and_create()
{
  rrm_status status;
  rrm_trash trash;

  if (clear() != 0) {
    return EXIT_FAILURE;
  }

  status = rrm_trash_open(&trash, "my_trash", true);
  if (rrm_status_is_error(status)) {
    return EXIT_FAILURE;
  }

  rrm_trash_close(&trash);

  if (access("my_trash", F_OK) != 0) {
    return EXIT_FAILURE;
  }

  clear();

  return EXIT_SUCCESS;
}