#pragma once

#ifndef RRM_DUMP_H
#define RRM_DUMP_H

#include "status.h"
#include <stdbool.h>
#include <time.h>

/**
 * We have to forward declare the trash, otherwise we would have a circular
 * dependency.
 */
struct rrm_trash;

/**
 * Each delete action creates a new dump, where the data is stored of this
 * particular action. The dump can be restored until it is permanently deleted.
 */
typedef struct rrm_dump
{
  struct rrm_trash *trash;
  char *path;
  const char *files_path;
  int dump_id;
  int lock_fd;
  int info_fd;
} rrm_dump;

/**
 * @brief Creates a new dump.
 *
 * This function creates a new dump. It returns an error if the specified dump
 * identifier already exists.
 *
 * @param trash The trash which owns this dump.
 * @param dump_id The unique identifier of this dump.
 * @return Returns RRM_SOK on success or an error otherwise.
 */
rrm_status rrm_dump_create(struct rrm_trash *trash, int dump_id);

/**
 * @brief Opens a dump.
 *
 * This function opens a dump contained in a specific trash. It will create a
 * new dump if create_if_missing is set to true and the dump doesn't exist - or
 * return an error if it exists and it is set to false. The opened dump needs to
 * be closed using rrm_dump_close if it is no longer needed.
 *
 * @param dump The dump which will be initialized.
 * @param trash The trash which owns this dump.
 * @param dump_id The unique dump identifier of the dump to be opened.
 * @param create_if_missing Indicates whether a dump will be created if it is
 * not found.
 * @return Returns RRM_SOK on success or an error otherwise.
 */
rrm_status rrm_dump_open(rrm_dump *dump, struct rrm_trash *trash, int dump_id,
  bool create_if_missing);

/**
 * @brief Moves a dump after the specified dump.
 *
 * This function moves a dump after a specified dump.
 *
 * @param dump The dump which is placed before the other dump.
 * @param new_dump The dump which follows the other dump.
 * @return Returns RRM_SOK on success or an error otherwise.
 */
rrm_status rrm_dump_move_after(rrm_dump *dump, rrm_dump *new_dump);

/**
 * @brief Moves to the next dump.
 *
 * This function moves to the next dump, following the current one. The handle
 * still needs to be closed after it is no longer required.
 *
 * @param dump The dump which will be moved to the next one.
 * @return Returns RRM_SOK on success or an error otherwise.
 */
rrm_status rrm_dump_next(rrm_dump *dump);

/**
 * @brief Moves to the previous dump.
 *
 * This function moves to the previous dump, following the current one. The
 * handle still needs to be closed after it is no longer required.
 *
 * @param dump The dump which will be moved to the next one.
 * @return Returns RRM_SOK on success or an error otherwise.
 */
rrm_status rrm_dump_previous(rrm_dump *dump);

/**
 * @brief Gets the dump identifier.
 *
 * This function extracts the unique dump identifier from the dump, which can be
 * used to open and inspect a dump.
 *
 * @param dump The dump which will be inspected.
 * @return Returns the dump identifier.
 */
int rrm_dump_get_id(const rrm_dump *dump);

/**
 * @brief Gets the time when a dump was created.
 *
 * This function extracts the time of a dump, which determines when this
 * specific dump was allocated.
 *
 * @param dump The dump which will be inspected.
 * @return Returns a time_t value which corresponds to the time when the dump
 * was created.
 */
time_t rrm_dump_get_time(const rrm_dump *dump);

/**
 * @brief Add file to the dump.
 *
 * This function moves a file to the dump. The new file will have the same path
 * under the dump.
 *
 * @param dump The dump where the file will be moved to.
 * @param file The file path of the file which will be moved to the dump.
 * @return Returns RRM_SOK on success or an error otherwise.
 */
rrm_status rrm_dump_add_file(rrm_dump *dump, const char *file);

/**
 * @brief Closes the dump.
 *
 * This function disposes the current dump and releases all resources acquired
 * by it.
 *
 * @param dump The dump which will be released.
 */
void rrm_dump_close(rrm_dump *dump);

#endif
