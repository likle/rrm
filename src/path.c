#include "path.h"
#include <memory.h>
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

  // First we determine the length of the two pieces which we will combine.
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
