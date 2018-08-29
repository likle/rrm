#include <constants.h>
#include <stdio.h>
#include <stdlib.h>
#include <trash.h>
#include <unistd.h>

static void clear_dumps()
{
  int i;
  char path[255];

  for (i = 0; i < 100; ++i) {
    sprintf(path, "my_trash/%i/" RRM_FILES_FOLDER_NAME, i);
    remove(path);
    sprintf(path, "my_trash/%i/" RRM_INFO_FILE_NAME, i);
    remove(path);
    sprintf(path, "my_trash/%i/" RRM_LOCK_FILE_NAME, i);
    remove(path);
    sprintf(path, "my_trash/%i", i);
    remove(path);
  }
}

static int clear()
{
  clear_dumps();
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

int trash_insert()
{
  rrm_status status;
  rrm_trash trash;
  rrm_dump dump;

  if (clear() != 0) {
    return EXIT_FAILURE;
  }

  status = rrm_trash_create("my_trash");
  if (rrm_status_is_error(status)) {
    goto err_create;
  }

  status = rrm_trash_open(&trash, "my_trash", false);
  if (rrm_status_is_error(status)) {
    goto err_open;
  }

  status = rrm_trash_insert(&trash, NULL, &dump);
  if (rrm_status_is_error(status)) {
    goto err_insert;
  }

  rrm_dump_close(&dump);

  status = rrm_trash_insert(&trash, NULL, &dump);
  if (rrm_status_is_error(status)) {
    goto err_insert;
  }

  rrm_dump_close(&dump);

  status = rrm_trash_begin(&trash, &dump);
  if (status != RRM_SOK) {
    goto err_begin;
  }

  status = rrm_dump_next(&dump);
  if (status != RRM_SOK) {
    goto err_next;
  }

  status = rrm_dump_next(&dump);
  if (status != RRM_SEND) {
    goto err_next;
  }

  rrm_dump_close(&dump);

  rrm_trash_close(&trash);
  clear();

  return EXIT_SUCCESS;

err_next:
  rrm_dump_close(&dump);
err_begin:
err_insert:
  rrm_trash_close(&trash);
err_open:
  clear();
err_create:
  return EXIT_FAILURE;
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
    goto err_create;
  }

  status = rrm_trash_open(&trash, "my_trash", false);
  if (rrm_status_is_error(status)) {
    goto err_open;
  }

  if (access("my_trash", F_OK) != 0) {
    goto err_access;
  }

  rrm_trash_close(&trash);
  clear();

  return EXIT_SUCCESS;

err_access:
  rrm_trash_close(&trash);
err_open:
  clear();
err_create:
  return EXIT_FAILURE;
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