#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils.h>

#define UNITS(XX) XX(option)

struct tsh_unit
{
  const char *name;
  int (*fn)(int argc, char *argv[]);
};

#define XX(u) extern int u##_test(int argc, char *argv[]);
UNITS(XX)
#undef XX

static struct tsh_unit units[] = {
#define XX(u) {.name = #u, .fn = u##_test},
  UNITS(XX)
#undef XX
};

int main(int argc, char *argv[])
{
  size_t i;
  const char *requested_unit_name;
  struct tsh_unit *unit;

  if (argc < 2) {
    for (i = 0; i <= TSH_ARRAY_SIZE(units); ++i) {
      unit = &units[i];
      if (unit->fn(argc, argv) == EXIT_FAILURE) {
        return EXIT_FAILURE;
      }
    }

    return EXIT_SUCCESS;
  }

  requested_unit_name = argv[1];
  for (i = 0; i <= TSH_ARRAY_SIZE(units); ++i) {
    unit = &units[i];
    if (strcmp(unit->name, requested_unit_name) == 0) {
      return unit->fn(argc, argv);
    }
  }

  fprintf(stderr, "Unknown unit '%s'.", requested_unit_name);

  return EXIT_FAILURE;
}