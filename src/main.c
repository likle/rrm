#include <locale.h>
#include <stdlib.h>

#include "constants.h"
#include "option.h"
#include "utils.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

struct rrm_config
{
  bool force : 1;
  bool recursive : 1;
};

static struct rrm_option options[] = {

  {.identifier = 'f',
    .access_letters = "f",
    .access_name = "force",
    .value_name = NULL,
    .description = "ignore non-existent files and arguments, never prompt"},

  {.identifier = 'r',
    .access_letters = "rR",
    .access_name = "recursive",
    .value_name = NULL,
    .description = "remove directories and their contents recursively"},

  {.identifier = 'd',
    .access_letters = "d",
    .access_name = NULL,
    .value_name = NULL,
    .description = "remove empty directories"},

  {.identifier = 'u',
    .access_letters = "u",
    .access_name = "undo",
    .value_name = NULL,
    .description = "recovers previously deleted dumps"},

  {.identifier = 'o',
    .access_letters = NULL,
    .access_name = "output",
    .value_name = "DIR",
    .description = "specifies an output directory for recovery"},

  {.identifier = 'l',
    .access_letters = "l",
    .access_name = "list",
    .value_name = NULL,
    .description = "lists previously deleted dumps"},

  {.identifier = 'v',
    .access_letters = "v",
    .access_name = "verbose",
    .value_name = NULL,
    .description = "explain what is being done"},

  {.identifier = 'h',
    .access_letters = NULL,
    .access_name = "help",
    .value_name = NULL,
    .description = "display this help and exit"},

  {.identifier = 'V',
    .access_letters = NULL,
    .access_name = "version",
    .value_name = NULL,
    .description = "output version info and exit"}

};

static void print_help(void)
{
  fputs("Usage: rrm [OPTION]... [FILE]...\n", stdout);
  fputs("   or: rrm [OPTION]... -l\n", stdout);
  fputs("   or: rrm [OPTION]... -u [DUMP]...\n", stdout);
  fputs("Moves files to the trash, recovers or lists dumps.\n\n", stdout);
  rrm_option_print(options, RRM_ARRAY_SIZE(options), stdout);

  fputs("\nEach delete is kept in a separate trash dump. The --list flag can "
        "be used to\n"
        "receive DUMP information about previous delete operations operations. "
        "Use the\n"
        "--undo flag to recover the files of a dump. If no --output is "
        "specified in\n"
        "combination with --undo, the original file path will be used.\n",
    stdout);
}

static void print_version(void)
{
  const char *build_year;

  build_year = __DATE__ + 7;
  fputs("rrm " RRM_VERSION "\n", stdout);
  fprintf(stdout, "Copyright (C) %s " RRM_AUTHOR "\n", build_year);
  fputs("License: MIT\n", stdout);
}

int main(int argc, char *argv[])
{
  struct rrm_config config;
  rrm_option_context ctx;
  char option;
  int index;

  setlocale(LC_ALL, "");

  rrm_option_prepare(&ctx, options, RRM_ARRAY_SIZE(options), argc, argv);

  while (rrm_option_fetch(&ctx)) {
    option = rrm_option_get(&ctx);
    index = rrm_option_get_index(&ctx);
    switch (option) {
    case 'r':
    case 'R':
      config.recursive = true;
      break;
    case 'f':
      config.force = true;
      break;
    case 'V':
      print_version();
      return EXIT_SUCCESS;
    case 'h':
      print_help();
      return EXIT_SUCCESS;
    case '?':
    default:
      fprintf(stderr,
        RRM_TARGET ": invalid option '%s'. \n"
                   "Try '" RRM_TARGET " --help' for more information. \n",
        argv[index - 1]);

      return EXIT_FAILURE;
    }
  }

  printf("%i", config.recursive);

  return EXIT_SUCCESS;
}
