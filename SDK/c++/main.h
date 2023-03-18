#pragma once

#include <iostream>

#include "config.h"
#include "workbench.h"

// #define DEBUG

#ifdef DEBUG
extern FILE* fp;
#define debug(fmt, ...) fprintf(fp, fmt, ##__VA_ARGS__)
#else
#define debug(fmt, ...)
#endif

extern int workbench_num;
extern workbench workbenches[WORKBENCH_NUM_MAX];

extern int need[MATERIAL_TYPE_NUM + 1];
extern int occupy[MATERIAL_TYPE_NUM + 1];
