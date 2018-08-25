#pragma once

#ifndef TSH_DUMP_H
#define TSH_DUMP_H

#include "trash.h"
#include <stdbool.h>

/**
 * Each delete action creates a new dump, where the data is stored of this
 * particular action. The dump can be restored until it is permanently deleted.
 */
typedef struct tsh_dump
{
  tsh_trash *trash;
} tsh_dump;

int tsh_dump_create(tsh_trash *trash);

int tsh_dump_delete(tsh_trash *trash, int dump_id);

int tsh_dump_open(tsh_dump *dump, tsh_trash *trash, int dump_id);

void tsh_dump_close(tsh_trash *trash);

#endif
