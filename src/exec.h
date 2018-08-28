#pragma once

#ifndef RRM_EXEC_H
#define RRM_EXEC_H

#include "status.h"
#include <stdio.h>

rrm_status rrm_exec(int argc, char *argv[], FILE *out, FILE *err);

#endif
