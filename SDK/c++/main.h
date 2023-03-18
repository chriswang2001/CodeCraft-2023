#pragma once

#include <iostream>

#include "config.h"
#include "work_bench.h"

#define DEBUG

#ifdef DEBUG
extern FILE* fp;
#define debug(fmt, ...) fprintf(fp, fmt, ##__VA_ARGS__)
#else
#define debug(fmt, ...)
#endif

extern int work_bench_num;
extern work_bench work_benches[WORK_BENCH_NUM_MAX];

extern int need[MATERIAL_TYPE_NUM + 1];
extern int occupy[MATERIAL_TYPE_NUM + 1];
