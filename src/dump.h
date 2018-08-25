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

rrm_status rrm_dump_create(rrm_trash *trash, int *dump_id);

int rrm_dump_delete(rrm_trash *trash, int dump_id);

int rrm_dump_open(rrm_dump *dump, rrm_trash *trash, int dump_id);

void rrm_dump_close(rrm_trash *trash);

#endif
