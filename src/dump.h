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
  int lock_fd;
  int info_fd;
} rrm_dump;

rrm_status rrm_dump_create(rrm_trash *trash, const char **files, int *dump_id);

rrm_status rrm_dump_delete(rrm_trash *trash, int dump_id);

rrm_status rrm_dump_recover(rrm_trash *trash, int dump_id, const char *output);

#endif
