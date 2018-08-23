#pragma once

#ifndef TSH_OPTION_H
#define TSH_OPTION_H

#include <stdbool.h>
#include <stddef.h>

typedef struct tsh_option
{
  const char identifier;
  const char *access_letters;
  const char *access_name;
  const char *value_name;
  const char *description;
} tsh_option;

typedef struct tsh_option_context
{
  const struct tsh_option *options;
  size_t option_count;
  int argc;
  char **argv;

  int index;
  int inner_index;
  bool forced_end;
  char identifier;
  char *value;
} tsh_option_context;

void tsh_option_print(const tsh_option *options, size_t option_count);

void tsh_option_prepare(tsh_option_context *context,
  const tsh_option *options, size_t option_count, int argc, char **argv);

bool tsh_option_fetch(tsh_option_context *context);

char tsh_option_get(const tsh_option_context *context);

const char *tsh_option_get_value(const tsh_option_context *context);

int tsh_option_get_index(const tsh_option_context *context);

#endif
