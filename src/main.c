#include "exec.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if (rrm_status_is_error(rrm_exec(argc, argv, stdout, stderr))) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
