#pragma once

#ifndef RRM_UTILS_H
#define RRM_UTILS_H

/**
 * This is just a small macro which calculates the size of an array.
 */
#define RRM_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/**
 * Used to remove warning for "unused parameters" or "unused variable". We do a
 * comparison here since some IDEs will still complain otherwise.
 */
#define RRM_UNUSED(p) (void)(p)

#endif
