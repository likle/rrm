#pragma once

#ifndef TSH_TRASH_H
#define TSH_TRASH_H

#include "status.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * A trash is a directory which contains deleted data.
 */
typedef struct tsh_trash
{
  const char *path;
  int lock_fd;
  int state_fd;
} tsh_trash;

/**
 * @brief Creates a new trash at the specified location.
 *
 * This function creates a new trash at the specified location. A call to this
 * function will fail if the trash already exits.
 *
 * @param path The path to the trash location.
 * @return Returns TSH_SOK on success or an error status otherwise.
 */
tsh_status tsh_trash_create(const char *path);

/**
 * @brief Opens an existing trash.
 *
 * This function opens an existing trash. The trash is opened in a concurrent
 * safe manner, which means that multiple processes may access it. A call to
 * this function will fail if a trash does not exist, unless create_if_missing
 * is set to true in which case a new trash is created.
 *
 * @param trash The trash object which will be initialized.
 * @param path A path to a trash directory.
 * @param create_if_missing A bool which indicates if a trash is created if it
 * doesn't exist.
 * @return Returns TSH_SOK on success or an error otherwise.
 */
tsh_status tsh_trash_open(tsh_trash *trash, const char *path,
  bool create_if_missing);

/**
 * @brief Closes a trash.
 *
 * This function disposes all resources required by a trash object. The trash
 * object must be initialize using tsh_tash_open before calling this function.
 *
 * @param trash The trash object which will be released.
 */
void tsh_trash_close(tsh_trash *trash);

#endif
