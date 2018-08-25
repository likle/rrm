#pragma once

#ifndef RRM_STATUS_H
#define RRM_STATUS_H

#define RRM_STATUS_MAP(XX)                                                     \
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
#define XX(n, e, m) RRM_S##n,
  RRM_STATUS_MAP(XX)
#undef XX
} rrm_status;

bool rrm_status_is_error(rrm_status status);

const char *rrm_status_get_string(rrm_status status);

rrm_status rrm_status_from_os(int err);

#endif
