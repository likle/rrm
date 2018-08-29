#include <limits.h>
#include <memory.h>
#include <path.h>
#include <stdint.h>
#include <stdlib.h>

int path_resolve_navigate_back()
{
  const char *absolute, *relative, *expected;
  char result[PATH_MAX];

  absolute = "/hello/this/is/absolute";
  relative = "../../must/work";
  expected = "/hello/this/must/work";
  rrm_path_resolve(absolute, relative, sizeof(result), result);

  if (strcmp(expected, result) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int path_resolve_absolute()
{
  const char *absolute, *relative, *expected;
  char result[PATH_MAX];

  absolute = "/hello/this/is/absolute";
  relative = "/another/absolute/path";
  expected = relative;
  rrm_path_resolve(absolute, relative, sizeof(result), result);

  if (strcmp(expected, result) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int path_resolve_current()
{
  const char *absolute, *relative, *expected;
  char result[PATH_MAX];

  absolute = "/hello/this/is/absolute";
  relative = "./fun";
  expected = "/hello/this/is/absolute/fun";
  rrm_path_resolve(absolute, relative, sizeof(result), result);

  if (strcmp(expected, result) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int path_resolve_simple()
{
  const char *absolute, *relative, *expected;
  char result[PATH_MAX];

  absolute = "/hello/this/is/absolute";
  relative = "fun";
  expected = "/hello/this/is/absolute/fun";
  rrm_path_resolve(absolute, relative, sizeof(result), result);

  if (strcmp(expected, result) != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int path_find_common_limited()
{
  const char *a, *b;
  size_t result;

  a = "abc/hello/there";
  b = "abc";
  result = rrm_path_find_common(a, b, 2);
  if (result != 0) {
    return EXIT_FAILURE;
  }

  a = "abc/hello/there";
  b = "abc";
  result = rrm_path_find_common(a, b, 3);
  if (result != 3) {
    return EXIT_FAILURE;
  }

  a = "abc/hello/there";
  b = "abc";
  result = rrm_path_find_common(a, b, 4);
  if (result != 3) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int path_find_common_similar()
{
  const char *a, *b;
  size_t result;

  a = "abc/hello/there";
  b = "abc_folder";
  result = rrm_path_find_common(a, b, SIZE_MAX);
  if (result != 0) {
    return EXIT_FAILURE;
  }

  a = "abc_folder";
  b = "abc/hello/there";
  result = rrm_path_find_common(a, b, SIZE_MAX);
  if (result != 0) {
    return EXIT_FAILURE;
  }

  a = "abc_folder/hello/there";
  b = "abc";
  result = rrm_path_find_common(a, b, SIZE_MAX);
  if (result != 0) {
    return EXIT_FAILURE;
  }

  a = "abc";
  b = "abc_folder/hello/there";
  result = rrm_path_find_common(a, b, SIZE_MAX);
  if (result != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int path_find_common_terminated()
{
  const char *a, *b;
  size_t result;

  a = "abc/hello/there";
  b = "abc";
  result = rrm_path_find_common(a, b, SIZE_MAX);
  if (result != 3) {
    return EXIT_FAILURE;
  }

  a = "abc";
  b = "abc/hello/there";
  result = rrm_path_find_common(a, b, SIZE_MAX);
  if (result != 3) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int path_find_common_simple()
{
  const char *a, *b;
  size_t result;

  a = "abc/hello/there";
  b = "abc/some_thing_else";
  result = rrm_path_find_common(a, b, SIZE_MAX);
  if (result != 3) {
    return EXIT_FAILURE;
  }

  a = "abc/some_thing_else";
  b = "abc/hello/there";
  result = rrm_path_find_common(a, b, SIZE_MAX);
  if (result != 3) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

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