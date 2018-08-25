#pragma once

#ifndef TSH_STATUS_H
#define TSH_STATUS_H

#define TSH_STATUS_MAP(XX)                                                     \
  XX(LOOP, true, "Symbolic link loop")                                         \
  XX(MLINK, true, "Too many links")                                            \
  XX(NOACS, true, "Access is denied")                                          \
  XX(NODIR, true, "Not a directory")                                           \
  XX(NOENT, true, "No such file or directory")                                 \
  XX(NOMEM, true, "Out of memory")                                             \
  XX(NOSPC, true, "Insufficient space")                                        \
  XX(NTLNG, true, "Name too long")                                             \
  XX(OK, false, "Everything is ok")                                            \
  XX(ROFS, true, "Read-only file system")                                      \
  XX(UNKWN, true, "Unknown error")

#include <stdbool.h>

typedef enum
{
#define XX(n, e, m) TSH_S##n,
  TSH_STATUS_MAP(XX)
#undef XX
} tsh_status;

bool tsh_status_is_error(tsh_status status);

const char *tsh_status_get_string(tsh_status status);

tsh_status tsh_status_from_os(int err);

#endif
