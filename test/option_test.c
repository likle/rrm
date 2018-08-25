#include <assert.h>
#include <limits.h>
#include <memory.h>
#include <option.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
#include <wordexp.h>

static struct rrm_option options[] = {

  {.identifier = 's',
    .access_letters = "s",
    .access_name = NULL,
    .value_name = NULL,
    .description = "Simple flag"},

  {.identifier = 'a',
    .access_letters = "a",
    .access_name = NULL,
    .value_name = NULL,
    .description = "Another simple flag"},

  {.identifier = 'm',
    .access_letters = "mMoO",
    .access_name = NULL,
    .value_name = NULL,
    .description = "Multiple access letters"},

  {.identifier = 'l',
    .access_letters = NULL,
    .access_name = "long",
    .value_name = NULL,
    .description = "Long parameter name"},

  {.identifier = 'k',
    .access_letters = "k",
    .access_name = "key",
    .value_name = "VALUE",
    .description = "Parameter value"}};

struct rrm_result
{
  bool simple_flag : 1;
  bool another : 1;
  bool multiaccess_flags : 1;
  bool long_parameter : 1;
  bool value_parameter : 1;
  bool unknown : 1;
  bool def : 1;
  const char *value;
};

static struct rrm_result result;
static wordexp_t wordexp_value;
static char **argv;
static int argc;

static int make_args(const char *str)
{
  int status;

  status = wordexp(str, &wordexp_value, 0);
  if (status != 0) {
    if (status == WRDE_NOSPACE) {
      wordfree(&wordexp_value);
    }

    return status;
  }

  assert(wordexp_value.we_wordc <= INT_MAX);
  argc = (int)wordexp_value.we_wordc;
  argv = wordexp_value.we_wordv;

  return 0;
}

static void destroy_args()
{
  wordfree(&wordexp_value);
}

static int option_test_run(int argc, char *argv[])
{
  int index;
  char identifier;
  rrm_option_context context;

  rrm_option_prepare(&context, options, RRM_ARRAY_SIZE(options), argc, argv);

  memset(&result, 0, sizeof(result));

  while (rrm_option_fetch(&context)) {
    identifier = rrm_option_get(&context);
    switch (identifier) {
    case 's':
      result.simple_flag = true;
      break;
    case 'a':
      result.another = true;
      break;
    case 'm':
      result.multiaccess_flags = true;
      break;
    case 'l':
      result.long_parameter = true;
      break;
    case 'k':
      result.value_parameter = true;
      result.value = rrm_option_get_value(&context);
      break;
    case '?':
      result.unknown = true;
      break;
    default:
      result.def = true;
      break;
    }
  }

  index = rrm_option_get_index(&context);
  if (rrm_option_fetch(&context) != false) {
    return -1;
  }

  if (rrm_option_get_index(&context) != index) {
    return -1;
  }

  return rrm_option_get_index(&context);
}

int option_complex(void)
{
  int status;

  status = make_args("test file1 -s -- -a -- a file");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (!result.simple_flag || result.another || result.multiaccess_flags ||
      result.long_parameter || result.value_parameter || result.unknown ||
      result.def || result.value != NULL) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_mixed(void)
{
  int status, i, j;
  const char *values[] = {"file1", "mixed", "file3", "file2", "-m",
    "parameters", "file4"};

  status = make_args(
    "test -s file1 -k=value file2 -a mixed file3 -- -m parameters file4");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0 || argc - status != 7) {
    goto err_wrong;
  }

  /* The following should come after the index:
   * --
   * file1
   * mixed
   * file3
   * file2
   * -m
   * parameters
   * file4
   */
  for (i = 0; i < RRM_ARRAY_SIZE(values); ++i) {
    for (j = status; j < argc; ++j) {
      if (strcmp(argv[j], values[i]) == 0) {
        goto found;
      }
    }

    goto err_wrong;
  found:;
  }

  if (!result.simple_flag || !result.another || result.multiaccess_flags ||
      result.long_parameter || !result.value_parameter || result.unknown ||
      result.def || result.value == NULL ||
      strcmp(result.value, "value") != 0) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_ending(void)
{
  int status;

  status = make_args("test -s -- -a");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (!result.simple_flag || result.another || result.multiaccess_flags ||
      result.long_parameter || result.value_parameter || result.unknown ||
      result.def || result.value != NULL) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_long_missing_value(void)
{
  int status;

  status = make_args("test --key");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple_flag || result.another || result.multiaccess_flags ||
      result.long_parameter || !result.value_parameter || result.unknown ||
      result.def || result.value != NULL) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_short_missing_value(void)
{
  int status;

  status = make_args("test -k");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple_flag || result.another || result.multiaccess_flags ||
      result.long_parameter || !result.value_parameter || result.unknown ||
      result.def || result.value != NULL) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_long_space_value(void)
{
  int status;

  status = make_args("test --key super_value");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple_flag || result.another || result.multiaccess_flags ||
      result.long_parameter || !result.value_parameter || result.unknown ||
      result.def || result.value == NULL ||
      strcmp(result.value, "super_value") != 0) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_short_space_value(void)
{
  int status;

  status = make_args("test -k test_value");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple_flag || result.another || result.multiaccess_flags ||
      result.long_parameter || !result.value_parameter || result.unknown ||
      result.def || result.value == NULL ||
      strcmp(result.value, "test_value") != 0) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_long_equal_value(void)
{
  int status;

  status = make_args("test --key=super_value");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple_flag || result.another || result.multiaccess_flags ||
      result.long_parameter || !result.value_parameter || result.unknown ||
      result.def || result.value == NULL ||
      strcmp(result.value, "super_value") != 0) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_short_equal_value(void)
{
  int status;

  status = make_args("test -k=test_value");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple_flag || result.another || result.multiaccess_flags ||
      result.long_parameter || !result.value_parameter || result.unknown ||
      result.def || result.value == NULL ||
      strcmp(result.value, "test_value") != 0) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_combined(void)
{
  int status;

  status = make_args("test -sma");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (!result.simple_flag || !result.another || !result.multiaccess_flags ||
      result.long_parameter || result.value_parameter || result.unknown ||
      result.def || result.value != NULL) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_unknown_long(void)
{
  int status;

  status = make_args("test --unknown");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple_flag || result.another || result.multiaccess_flags ||
      result.long_parameter || result.value_parameter || !result.unknown ||
      result.def || result.value != NULL) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_unknown_short(void)
{
  int status;

  status = make_args("test -u");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple_flag || result.another || result.multiaccess_flags ||
      result.long_parameter || result.value_parameter || !result.unknown ||
      result.def || result.value != NULL) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_alias(void)
{
  int status;

  status = make_args("test -O");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple_flag || result.another || !result.multiaccess_flags ||
      result.long_parameter || result.value_parameter || result.unknown ||
      result.def || result.value != NULL) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_simple_long(void)
{
  int status;

  status = make_args("test --long");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (result.simple_flag || result.another || result.multiaccess_flags ||
      !result.long_parameter || result.value_parameter || result.unknown ||
      result.def || result.value != NULL) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_simple(void)
{
  int status;

  status = make_args("test -s");
  if (status != 0) {
    goto err_setup;
  }

  status = option_test_run(argc, argv);
  if (status < 0) {
    goto err_wrong;
  }

  if (!result.simple_flag || result.another || result.multiaccess_flags ||
      result.long_parameter || result.value_parameter || result.unknown ||
      result.def || result.value != NULL) {
    goto err_wrong;
  }

  destroy_args();

  return EXIT_SUCCESS;

err_wrong:
  destroy_args();
err_setup:
  return EXIT_FAILURE;
}

int option_print(void)
{
  int status;
  char buf[255];
  char *result;
  const char *expected;
  const char *test_file_name;
  FILE *test_file;

  test_file_name = "option_print.out";
  expected = "  -s                   Simple flag\n"
             "  -a                   Another simple flag\n"
             "  -m, -M, -o, -O       Multiple access letters\n"
             "  --long               Long parameter name\n"
             "  -k, --key=VALUE      Parameter value\n";

  remove(test_file_name);
  test_file = fopen(test_file_name, "w+");
  if (test_file == NULL) {
    goto err_open;
  }

  rrm_option_print(options, RRM_ARRAY_SIZE(options), test_file);
  if (fseek(test_file, 0, SEEK_SET) != 0) {
    goto err_seek;
  }

  if (fread(buf, sizeof(buf), 1, test_file) != 1 && feof(test_file) == 0) {
    goto err_read;
  }

  if (memcmp(buf, expected, strlen(expected)) != 0) {
    goto err_test;
  }

  remove(test_file_name);
  fclose(test_file);
  return EXIT_SUCCESS;

err_test:
err_read:
err_seek:
  remove(test_file_name);
  fclose(test_file);
err_open:
  return EXIT_FAILURE;
}