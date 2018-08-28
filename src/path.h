#pragma once

#ifndef RRM_PATH_H
#define RRM_PATH_H

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
 * This function compares the two submitted paths a and b and finds common paths
 * in it. It will not compare more than n values.
 *
 * @param a The first path.
 * @param b The second path.
 * @param n The maximum amount of characters compared.
 * @return Returns the amount of characters which are common in both paths.
 */
size_t rrm_path_find_common(const char *a, const char *b, size_t n);

#endif
