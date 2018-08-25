#pragma once

#ifndef TSH_UTILS_H
#define TSH_UTILS_H

/**
 * This is just a small macro which calculates the size of an array.
 */
#define TSH_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/**
 * Used to remove warning for "unused parameters" or "unused variable". We do a
 * comparison here since some IDEs will still complain otherwise.
 */
#define TSH_UNUSED(p) (void)(p)

#endif
