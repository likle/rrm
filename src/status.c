#include "status.h"
#include "utils.h"
#include <assert.h>

static bool rrm_status_error_table[] = {
#define XX(c, e, t) e,
  RRM_STATUS_MAP(XX)
#undef XX
};

static const char *rrm_status_text_table[] = {
#define XX(c, e, t) t,
  RRM_STATUS_MAP(XX)
#undef XX
};

bool rrm_status_is_error(rrm_status status)
{
  assert(status > 0);
  return rrm_status_error_table[status];
}

const char *rrm_status_get_string(rrm_status status)
{
  assert(status > 0);
  return rrm_status_text_table[status];
}

rrm_status rrm_status_from_os(int err)
{
  RRM_UNUSED(err);
  return RRM_SUNKWN;
}
