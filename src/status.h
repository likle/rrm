#pragma once

#ifndef RRM_STATUS_H
#define RRM_STATUS_H

/**
 * This is a list of all status codes which are available.
 */
#define RRM_STATUS_MAP(XX)                                                     \
  XX(LOOP, true, "Symbolic link loop")                                         \
  XX(MLINK, true, "Too many links")                                            \
  XX(NOACS, true, "Access is denied")                                          \
  XX(NODIR, true, "Not a directory")                                           \
  XX(NOENT, true, "No such file or directory")                                 \
  XX(NOMEM, true, "Out of memory")                                             \
  XX(NOSPC, true, "Insufficient space")                                        \
  XX(NTLNG, true, "Name too long")                                             \
  XX(END, false, "End reached")                                                \
  XX(OK, false, "Everything is ok")                                            \
  XX(AEXST, true, "Already exists")                                            \
  XX(ROFS, true, "Read-only file system")                                      \
  XX(UNKWN, true, "Unknown error")

#include <stdbool.h>

typedef enum
{
#define XX(n, e, m) RRM_S##n,
  RRM_STATUS_MAP(XX)
#undef XX
} rrm_status;

/**
 * @brief Checks whether the specified status is an error.
 *
 * This function checks whether the specified status is an error. Since not all
 * status codes are errors, (e.g. RRM_SEND is no error), this function can be
 * used for error checking.
 *
 * @param status The status which will be verified.
 * @return Returns true if the status is an error or false otherwise.
 */
bool rrm_status_is_error(rrm_status status);

/**
 * @brief Gets a string corresponding to the status.
 *
 * This function gets a string corresponding to the status.
 *
 * @param status The status which will be used to lookup the string.
 * @return Returns a char pointer to the status string.
 */
const char *rrm_status_get_string(rrm_status status);

/**
 * @brief Maps a operating system status with a internal status.
 *
 * This function maps an operating system status with an internal status which
 * can be used to return an rrm_status from a function.
 *
 * @param err The operating system error code.
 * @return Returns a rrm_status which corresponds to the submitted os error.
 */
rrm_status rrm_status_from_os(int err);

#endif
