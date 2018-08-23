#pragma once

#ifndef TSH_TRASH_H
#define TSH_TRASH_H

#include <stdbool.h>

typedef struct tsh_trash
{
  const char *path;
} tsh_trash;

int tsh_trash_open(tsh_trash *trash, const char *path);

int tsh_trash_dump(char **files, bool force, bool recursive,
  bool delete_empty_directories);

int tsh_trash_restore(int dump_id);

void tsh_trash_close(tsh_trash *trash);

#endif
