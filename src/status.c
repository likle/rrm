#include "status.h"
#include "utils.h"
#include <assert.h>

static bool tsh_status_error_table[] = {
#define XX(c, e, t) e,
  TSH_STATUS_MAP(XX)
#undef XX
};

static const char *tsh_status_text_table[] = {
#define XX(c, e, t) t,
  TSH_STATUS_MAP(XX)
#undef XX
};

bool tsh_status_is_error(tsh_status status)
{
  assert(status > 0);
  return tsh_status_error_table[status];
}

const char *tsh_status_get_string(tsh_status status)
{
  assert(status > 0);
  return tsh_status_text_table[status];
}

tsh_status tsh_status_from_os(int err)
{
  TSH_UNUSED(err);
  return TSH_SUNKWN;
}
