#include <memory.h>
#include <path.h>
#include <stdlib.h>

int path_slash_suffix()
{
  const char *a, *b, *expected;
  char *result;

  a = "abc/";
  b = "def/";
  expected = "abc/def/";
  result = rrm_path_join(a, b);

  if (strcmp(result, expected) != 0) {
    free(result);
    return EXIT_FAILURE;
  }

  free(result);
  return EXIT_SUCCESS;
}

int path_slash_prefix()
{
  const char *a, *b, *expected;
  char *result;

  a = "/abc";
  b = "/def";
  expected = "/abc/def";
  result = rrm_path_join(a, b);

  if (strcmp(result, expected) != 0) {
    free(result);
    return EXIT_FAILURE;
  }

  free(result);
  return EXIT_SUCCESS;
}

int path_simple()
{
  const char *a, *b, *expected;
  char *result;

  a = "abc";
  b = "def";
  expected = "abc/def";
  result = rrm_path_join(a, b);

  if (strcmp(result, expected) != 0) {
    free(result);
    return EXIT_FAILURE;
  }

  free(result);
  return EXIT_SUCCESS;
}