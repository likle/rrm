#pragma once

#ifndef TSH_OPTION_H
#define TSH_OPTION_H

#include <stdbool.h>
#include <stddef.h>

/**
 * This is a simple alternative cross-platform implementation of getopt, which
 * parses argument strings.
 */

/**
 * An option is used to describe a flag/argument option submitted when the
 * program is run.
 */
typedef struct tsh_option
{
  const char identifier;
  const char *access_letters;
  const char *access_name;
  const char *value_name;
  const char *description;
} tsh_option;

/**
 * A context is used to iterate over all options provided. It stores the parsing
 * state.
 */
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

/**
 * @brief Prints all options to the terminal.
 *
 * This function prints all options to the terminal. This is used to generate
 * the output for a "--help" option.
 *
 * @param options The options which will be printed.
 * @param option_count The option count which will be printed.
 */
void tsh_option_print(const tsh_option *options, size_t option_count);

/**
 * @brief Prepare argument options for parsing.
 *
 * This function prepares the context for iteration and initializes the context
 * wit the supplied options and arguments. After the context has been prepared,
 * it can be used to fetch arguments from it.
 *
 * @param context The context which will be initialized.
 * @param options The registered options which are available for the program.
 * @param option_count The amount of options which are available for the
 * program.
 * @param argc The amount of arguments the user supplied in the main function.
 * @param argv A pointer to the arguments of the main function.
 */
void tsh_option_prepare(tsh_option_context *context, const tsh_option *options,
  size_t option_count, int argc, char **argv);

/**
 * @brief Fetches an option from the argument list.
 *
 * This function fetches a single option from the argument list. The context
 * will be moved to that item. Information can be extracted from the context
 * after the item has been fetched.
 * The arguments will be re-ordered, which means that non-option arguments will
 * be moved to the end of the argument list. After all options have been
 * fetched, all non-option arguments will be positioned after the index of
 * the context.
 *
 * @param context The context from which we will fetch the option.
 * @return Returns true if there was another option or false if the end is
 * reached.
 */
bool tsh_option_fetch(tsh_option_context *context);

/**
 * @brief Gets the identifier of the option.
 *
 * This function gets the identifier of the option, which should be unique to
 * this option and can be used to determine what kind of option this is.
 *
 * @param context The context from which the option was fetched.
 * @return Returns the identifier of the option.
 */
char tsh_option_get(const tsh_option_context *context);

/**
 * @brief Gets the value from the option.
 *
 * This function gets the value from the option, if any. If the option does not
 * contain a value, this function will return NULL.
 *
 * @param context The context from which the option was fetched.
 * @return Returns a pointer to the value or NULL if there is no value.
 */
const char *tsh_option_get_value(const tsh_option_context *context);

/**
 * @brief Gets the current index of the context.
 *
 * This function gets the current index within the argv arguments of the
 * context. This is particularly useful to inspect the original argument array,
 * or to get non-option arguments after option fetching has finished.
 *
 * @param context The context from which the option was fetched.
 * @return Returns the current index of the context.
 */
int tsh_option_get_index(const tsh_option_context *context);

#endif
