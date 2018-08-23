#include <locale.h>
#include <stdlib.h>

#include "constants.h"
#include "option.h"
#include "utils.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

struct tsh_config
{
  bool force : 1;
  bool recursive : 1;
};

static struct tsh_option options[] = {

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

  {.identifier = 'V',
    .access_letters = NULL,
    .access_name = "version",
    .value_name = NULL,
    .description = "output version info and exit"},

  {.identifier = 'h',
    .access_letters = NULL,
    .access_name = "help",
    .value_name = NULL,
    .description = "display this help and exit"}

};
/*
int run(char **files)
{
  int bitflags;
  FTSENT *ent;
  FTS *fts;

  bitflags = (FTS_NOSTAT | FTS_PHYSICAL);
  fts = fts_open(files, bitflags, NULL);
  while (1) {
    ent = fts_read(fts);
    if (ent == NULL) {
      break;
    }

    if (ent->fts_info == FTS_D) {
      printf("ND%.*s\n", ent->fts_namelen, ent->fts_name);
    } else {
      printf("D%.*s\n", ent->fts_namelen, ent->fts_name);
    }
  }
  // Indicate that the operation was successful.
  return EXIT_SUCCESS;
}*/

static void print_help(void)
{
  fputs("Usage: tsh [OPTION]... [FILE]...\n", stdout);
  fputs("Moves the files to the trash.\n\n", stdout);
  tsh_option_print(options, TSH_ARRAY_SIZE(options));
  fputs("\nBy default, tsh does not remove directories. Use the --recursive\n"
        "option to remove each listed directory too, along with all of its\n"
        "contents.",
    stdout);
}

static void print_version(void)
{
  const char *build_year;

  build_year = __DATE__ + 7;
  fputs("tsh " TSH_VERSION "\n", stdout);
  fprintf(stdout, "Copyright (C) %s " TSH_AUTHOR "\n", build_year);
  fputs("License: MIT\n", stdout);
}

int main(int argc, char *argv[])
{
  struct tsh_config config;
  tsh_option_context ctx;
  char option;
  int index;

  setlocale(LC_ALL, "");

  tsh_option_prepare(&ctx, options, TSH_ARRAY_SIZE(options), argc, argv);

  while (tsh_option_fetch(&ctx)) {
    option = tsh_option_get(&ctx);
    index = tsh_option_get_index(&ctx);
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
        TSH_TARGET ": invalid option '%s'. \n"
                   "Try '" TSH_TARGET " --help' for more information. \n",
        argv[index - 1]);

      return EXIT_FAILURE;
    }
  }

  printf("%i", config.recursive);

  return EXIT_SUCCESS;
}
