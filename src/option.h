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
  char identifier;
  char *value;
} tsh_option_context;

void tsh_option_print(const tsh_option *options, size_t option_count);

void tsh_option_prepare(tsh_option_context *context,
  const tsh_option *options, size_t option_count, int argc, char **argv);

bool tsh_option_fetch(tsh_option_context *context);

char tsh_option_get(tsh_option_context *context);

const char *tsh_option_get_value(tsh_option_context *context);

size_t tsh_option_get_value_size(tsh_option_context *context);

#endif
