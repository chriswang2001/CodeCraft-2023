#pragma once

#define FRAME_MAX 9000
#define FRAME_LIMIT 500

#define MAP_INPUT_LEN 100
#define MAP_MAX_LEN 50.0

#define ROBOT_NUM 4

#define MAX_LINEAR_VEL 6.0
#define MIN_LINEAR_VEL -2.0

#define MAX_ANGULAR_VEL M_PI
#define MIN_ANGULAR_VEL -M_PI

#define ANGULAR_LIMIT 0.15
#define DISTANCE_LIMIT 0.5

#define WORKBENCH_NUM_MAX 50
#define WORKBENCH_TYPE_NUM 9
#define MATERIAL_TYPE_NUM 7

#define MAX_TRACTION 250
#define MAX_MONMENT 50

extern int material[WORKBENCH_TYPE_NUM + 1];

extern int profit[MATERIAL_TYPE_NUM + 1];

extern int find[MATERIAL_TYPE_NUM + 1][WORKBENCH_TYPE_NUM];

double factor(double x, double max_x, double min_rate);

int bitcount(int num);
void bitcount(int num, int* arr);
