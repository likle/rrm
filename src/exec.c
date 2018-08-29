#include "exec.h"
#include <locale.h>
#include <stdlib.h>

#include "constants.h"
#include "option.h"
#include "trash.h"
#include "utils.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <memory.h>
#include <limits.h>

enum rrm_exec_task
{
  RRM_EXEC_HELP,
  RRM_EXEC_VERSION,
  RRM_EXEC_REMOVE,
  RRM_EXEC_RECOVER,
  RRM_EXEC_LIST,
};

struct rrm_exec_context
{
  enum rrm_exec_task task;
  char **files;
  const char *output;
  bool verbose;
};

static struct rrm_option options[] = {

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

static void rrm_exec_help(FILE *out)
{
  fputs("Usage: rrm [OPTION]... [FILE]...\n", out);
  fputs("   or: rrm [OPTION]... -l\n", out);
  fputs("   or: rrm [OPTION]... -u [DUMP]...\n", out);
  fputs("Moves files to the trash, recovers or lists dumps.\n\n", out);
  rrm_option_print(options, RRM_ARRAY_SIZE(options), out);

  fputs("\nEach delete is kept in a separate trash dump. The --list flag can "
        "be used to\n"
        "receive DUMP information about previous delete operations operations. "
        "Use the\n"
        "--undo flag to recover the files of a dump. If no --output is "
        "specified in\n"
        "combination with --undo, the original file path will be used.\n",
    out);
}

static void rrm_exec_version(FILE *out)
{
  const char *build_year;

  build_year = __DATE__ + 7;
  fputs("rrm " RRM_VERSION "\n", out);
  fprintf(out, "Copyright (C) %s " RRM_AUTHOR "\n", build_year);
  fputs("License: MIT\n", out);
}

static rrm_status rrm_exec_remove(struct rrm_exec_context *exec,
  rrm_trash *trash)
{
  rrm_dump dump;
  char **file;

  rrm_trash_insert(trash, NULL, &dump);
  while (*(file = exec->files++)) {
    rrm_dump_add_file(&dump, *file);
  }
  rrm_dump_close(&dump);

  return RRM_SOK;
}

static rrm_status rrm_exec_list(rrm_trash *trash, FILE *out)
{
  rrm_status status;
  rrm_dump dump;
  time_t timestamp;
  struct tm *time;
  char time_buf[70];
  char origin[PATH_MAX];

  status = rrm_trash_begin(trash, &dump);
  while (status == RRM_SOK) {
    timestamp = rrm_dump_get_time(&dump);
    rrm_dump_get_origin(&dump, origin);
    time = localtime(&timestamp);
    if(!strftime(time_buf, sizeof(time_buf), "%Y/%m/%d %H:%M:%S", time)) {
      strcpy(time_buf, "unknown_time");
    }

    fprintf(out, "[%03i] %s %s\n", rrm_dump_get_id(&dump), time_buf, origin);
    status = rrm_dump_next(&dump);
    if (rrm_status_is_error(status)) {
      goto err_next;
    }
  }

  return RRM_SOK;

err_next:
  rrm_dump_close(&dump);
  return status;
}

static rrm_status rrm_exec_select(struct rrm_exec_context *exec, FILE *out,
  FILE *err)
{
  rrm_status status;
  rrm_trash trash;

  status = rrm_trash_open(&trash, ".rrm", true);
  if (rrm_status_is_error(status)) {
    goto err_open;
  }

  switch (exec->task) {
  case RRM_EXEC_HELP:
    rrm_exec_help(out);
    break;
  case RRM_EXEC_VERSION:
    rrm_exec_version(out);
    break;
  case RRM_EXEC_REMOVE:
    rrm_exec_remove(exec, &trash);
    break;
  case RRM_EXEC_RECOVER:
    break;
  case RRM_EXEC_LIST:
    rrm_exec_list(&trash, out);
    break;
  default:
    fprintf(err, "error: unknown task.\n");
  }

  rrm_trash_close(&trash);

  return RRM_SOK;

err_open:
  return status;
}

rrm_status rrm_exec(int argc, char **argv, FILE *out, FILE *err)
{
  struct rrm_exec_context exec;
  rrm_option_context ctx;
  char option;
  int index;
  const char *value;

  setlocale(LC_ALL, "");

  index = 1;

  exec.task = RRM_EXEC_REMOVE;

  rrm_option_prepare(&ctx, options, RRM_ARRAY_SIZE(options), argc, argv);
  while (rrm_option_fetch(&ctx)) {

    option = rrm_option_get(&ctx);
    index = rrm_option_get_index(&ctx);
    value = rrm_option_get_value(&ctx);

    switch (option) {
    case 'u':
      exec.task = RRM_EXEC_RECOVER;
      break;
    case 'o':
      exec.output = value;
      break;
    case 'l':
      exec.task = RRM_EXEC_LIST;
      break;
    case 'v':
      exec.verbose = true;
      break;
    case 'h':
      exec.task = RRM_EXEC_HELP;
      break;
    case 'V':
      exec.task = RRM_EXEC_HELP;
      break;
    case '?':
    default:
      fprintf(stderr,
        RRM_TARGET ": invalid option '%s'. \n"
                   "Try '" RRM_TARGET " --help' for more information. \n",
        argv[index - 1]);

      return RRM_SUNKWN;
    }
  }

  exec.files = &argv[index];
  rrm_exec_select(&exec, out, err);

  return RRM_SOK;
}
