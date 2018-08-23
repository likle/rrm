#include "trash.h"

int tsh_trash_open(tsh_trash *trash, const char *path)
{
  trash->path = path;

  return 0;
}
