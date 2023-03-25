#pragma once

#include "config.h"
#include "robot.h"
#include "workbench.h"

extern int frameID, preFrameID;

extern int workbench_num;
extern workbench workbenches[WORKBENCH_NUM_MAX];

extern robot robots[ROBOT_NUM];

extern std::list<int> need[MATERIAL_TYPE_NUM + 1];
extern double want[MATERIAL_TYPE_NUM + 1];