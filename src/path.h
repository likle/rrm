#pragma once

#ifndef RRM_PATH_H
#define RRM_PATH_H

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

#endif
