#pragma once

#ifndef TSH_TRASH_H
#define TSH_TRASH_H

#include <stdbool.h>

typedef struct tsh_trash
{
  const char *path;
} tsh_trash;

struct tsh_trash_dump_info {
  bool force;
  bool recursive;
  bool delete_empty_directories;
};

int tsh_trash_open(tsh_trash *trash, const char *path);

int tsh_trash_dump(tsh_trash *trash, char **files, bool force, bool recursive,
  bool delete_empty_directories);

int tsh_trash_restore(int dump_id);

int tsh_trash_purge();

void tsh_trash_close(tsh_trash *trash);

#endif
