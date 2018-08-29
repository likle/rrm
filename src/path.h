#pragma once

#ifndef RRM_PATH_H
#define RRM_PATH_H

#include "status.h"
#include <stddef.h>

/**
 * @brief Joins two paths together.
 *
 * This function joins two paths together, separating them with a slash '/'. It
 * will not append a slash to the first segment if it already ends with one. The
 * returned value has to be freed.
 *
 * @param a The first segment of the resulting path.
 * @param b The second segment of the resulting path.
 * @return Returns a heap-allocated, null-terminated path string.
 */
char *rrm_path_join(const char *a, const char *b);

/**
 * @brief Finds common prefixes in two paths.
 *
 * This function compares the two submitted paths a and b and finds common path
 * segments in it. It will not compare more than n characters.
 *
 * @param a The first path.
 * @param b The second path.
 * @param n The maximum amount of characters compared.
 * @return Returns the amount of characters which are common in both paths.
 */
size_t rrm_path_find_common(const char *a, const char *b, size_t n);

/**
 * @brief Resolves a relative path using a base directory.
 *
 * This function resolves a relative path using a base directory. This is only a
 * string function and works independent from what is actually available on
 * disk.
 *
 * @param base The base directory from which the relative path originates.
 * @param relative The relative path based on the base directory.
 * @param result_size The result size
 * @param result The output of the absolute path.
 * @return Returns the amount of characters the result would take.
 */
size_t rrm_path_resolve(const char *base, const char *relative,
  size_t result_size, char *result);

#endif
