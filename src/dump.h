#pragma once

#ifndef RRM_DUMP_H
#define RRM_DUMP_H

#include "trash.h"
#include <stdbool.h>

/**
 * Each delete action creates a new dump, where the data is stored of this
 * particular action. The dump can be restored until it is permanently deleted.
 */
typedef struct rrm_dump
{
  rrm_trash *trash;
  char *path;
  int lock_fd;
  int info_fd;
} rrm_dump;

rrm_status rrm_dump_create(rrm_trash *trash, int dump_id);

rrm_status rrm_dump_open(rrm_dump *dump, rrm_trash *trash, int dump_id,
  bool create_if_missing);

rrm_status rrm_dump_configure(rrm_dump *dump, int next_dump_id,
  int prev_dump_id, bool active);

rrm_status rrm_dump_add_files(rrm_dump *dump, const char **files);

rrm_status rrm_dump_drop(rrm_dump *dump);

rrm_status rrm_dump_recover(rrm_dump *dump, char *output);

void rrm_dump_close(rrm_dump *dump);

#endif
