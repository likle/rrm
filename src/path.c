#include "path.h"
#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

char *rrm_path_join(const char *a, const char *b)
{
  size_t a_length, b_length;
  char *path;

  // First we remove the slashes from the second segment, since we don't want to
  // have any there.
  while (*b == '/') {
    ++b;
  }

  // Now we determine the length of the two pieces which we will combine.
  // That's as simple as calling strlen.
  a_length = strlen(a);
  b_length = strlen(b);

  // Now we allocate the memory which we require to generate the new path.
  // Theoretically it would be possible to do that on the stack, but those paths
  // could be rather long. So, the length of this memory block will be the two
  // string lengths plus 2 characters (a '\0' and possibly a '/' between the two
  // strings).
  path = malloc(a_length + b_length + 2);
  if (path == NULL) {
    return NULL;
  }

  // We copy the first path to the newly allocated block.
  memcpy(path, a, a_length);

  // Check whether the last symbol of the string is a '/', if not, we will
  // append one.
  if (path[a_length - 1] != '/') {
    path[a_length] = '/';
    ++a_length;
  }

  // And now we copy the second string to our new path.
  memcpy(path + a_length, b, b_length);

  // Finally, append a '\0' to the end of the path.
  path[a_length + b_length] = '\0';

  // Return the pointer to the caller. This will have to be disposed by the
  // caller though.
  return path;
}

size_t rrm_path_find_common(const char *a, const char *b, size_t n)
{
  size_t i, last;

  last = 0;
  for (i = 0;; ++i) {
    if (a[i] == '\0' || b[i] == '\0') {
      // So one of those strings reached the end. Now it might be that the other
      // string contains a dash, which would mean it is part of the common sub
      // path.
      if (a[i] == '/' || b[i] == '/') {
        last = i;
      }

      // In any case, since one string reached the end we have to leave at this
      // point.
      break;
    } else if (a[i] == '/' && b[i] == '/') {
      // If both strings point to a slash, this is part of the common sub path.
      last = i;
    }

    // We are done if the two strings differ, since from that point their path
    // obviously is no longer the same.
    if (a[i] != b[i]) {
      break;
    }

    // We need to stop scanning if we reached the limit.
    if (i >= n) {
      break;
    }
  }

  return last;
}

static bool rrm_path_apply_special_segment(const char *relative,
  char *result_begin, char *result_end, const char **new_relative,
  char **new_result_end)
{
  // First we check whether the segment starts with a dot. If it doesn't start
  // with a dot, it means it is no special segment.
  if (*relative != '.') {
    return false;
  }

  ++relative;
  if (*relative == '\0') {
    // This was just a dot. We will ignore it.
    *new_relative = relative;
    return false;
  } else if (*relative == '/') {
    // The relative path is a "./" which means "current folder". We just skip
    // it.
    ++relative;
    *new_relative = relative;
    return true;
  } else if (*relative != '.') {
    // This must be a hidden folder. We will not treat this as a special segment
    // since it doesn't change the path.
    return false;
  }

  // Now this must be a dot. This means there are two ".." following. We now
  // verify whether the segment ends after this or not.
  assert(*relative == '.');
  ++relative;

  if (*relative == '/') {
    ++relative;
  } else if (*relative != '\0') {
    // This is a simple file or folder starting with "..". We will ignore it and
    // not do anything, since this is not a special segment.
    return false;
  }

  // We swallowed the ".." segment. Now we need to indicate that by positioning
  // the new relative after it.
  *new_relative = relative;

  // The end of the result always has to be after the beginning, since we don't
  // allow empty strings.
  assert(result_end > result_begin);

  // We navigate one back in the result. This always has to be a slash, since
  // this function requires that the absolute path is ending with a slash.
  --result_end;
  assert(*result_end == '/');

  // Now we move back until we find the previous slash or we reached the
  // beginning.
  while (*--result_end != '/') {
    if (result_end <= result_begin) {
      *new_result_end = result_begin;
      return true;
    }
  }

  // Now we move after the slash.
  assert(*result_end == '/');
  *new_result_end = ++result_end;

  return true;
}

size_t rrm_path_resolve(const char *base, const char *relative,
  size_t result_size, char *result)
{
  size_t i;
  char *out;

  // If the relative path is actually an absolute path, we will just copy it
  // into the output and exit.
  if (*relative == '/') {
    strncpy(result, relative, result_size);
    return strlen(relative);
  }

  // Okay, so the user submitted a relative path. First we will copy the whole
  // base to the output.
  out = result;
  for (i = 0; i < result_size && base[i]; ++i) {
    *out++ = base[i];
  }

  // Make sure the absolute path ends with a slash '/'. If the base path is
  // string empty, we will change it to '/' as well.
  if ((i < result_size) && (i == 0 || base[i - 1] != '/')) {
    *out++ = '/';
  }

  // TODO verify result_size

  // The relative path can not start with a slash, since otherwise we would
  // already have left this function.
  assert(*relative != '/');

  do {
    if (rrm_path_apply_special_segment(relative, result, out, &relative,
          &out)) {
      continue;
    }

    while (*relative) {
      *out++ = *relative++;
      if (*relative == '/') {
        break;
      }
    }
  } while (*relative != '\0');

  // Finally, append a '\0' to indicate the end of the string.
  *out = '\0';

  return strlen(result);
}
