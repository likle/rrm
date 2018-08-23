#include <assert.h>
#include <limits.h>
#include <memory.h>
#include <option.h>
#include <stdlib.h>
#include <utils.h>
#include <wordexp.h>

static struct tsh_option options[] = {

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

struct tsh_result
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

static struct tsh_result result;
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

static void option_test_run(int argc, char *argv[])
{
  char identifier;
  tsh_option_context context;

  tsh_option_prepare(&context, options, TSH_ARRAY_SIZE(options), argc, argv);

  memset(&result, 0, sizeof(result));

  while (tsh_option_fetch(&context)) {
    identifier = tsh_option_get(&context);
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
      result.value = tsh_option_get_value(&context);
      break;
    case '?':
      result.unknown = true;
      break;
    default:
      result.def = true;
      break;
    }
  }
}

int option_args(void)
{
  int status;

  status = make_args("test -k=test_value");
  if (status != 0) {
    goto err_setup;
  }

  option_test_run(argc, argv);

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

  option_test_run(argc, argv);

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

int option_simple(void)
{
  int status;

  status = make_args("test -s");
  if (status != 0) {
    goto err_setup;
  }

  option_test_run(argc, argv);

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