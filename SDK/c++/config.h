#pragma once

#include <iostream>
#include <list>

// #define DEBUG

#ifdef DEBUG
extern FILE* fp;
#define debug(fmt, ...) fprintf(fp, fmt, ##__VA_ARGS__)
#else
#define debug(fmt, ...)
#endif

#define FRAME_MAX 9000
#define FRAME_LIMIT 30

#define FRAME_TIME 0.02
#define FRAME_PER_SECOND 50.0

#define MAP_INPUT_LEN 100
#define MAP_MAX_LEN 50.0

#define MAX_DISTACNE 70.710678
#define ROBOT_NUM 4

#define MAX_LINEAR_VEL 6.0
#define MIN_LINEAR_VEL -2.0

#define MAX_ANGULAR_VEL M_PI
#define MIN_ANGULAR_VEL -M_PI

#define ANGULAR_LIMIT 0.05
#define DISTANCE_LIMIT 4.5
#define BORDER_LIMIT 0.7

#define ROBOT_RADIUS_WITH_MATERIAL 0.53
#define ROBOT_RADIUS_WITHOUT_MATERIAL 0.45

#define WORKBENCH_NUM_MAX 50
#define WORKBENCH_TYPE_NUM 9
#define MATERIAL_TYPE_NUM 7

#define DENSITY 20
#define MAX_TRACTION 250
#define MAX_MONMENT 50

#define ANGULAR_PID_P 18.0
#define ANGULAR_PID_I 0.0
#define ANGULAR_PID_D 0.0

extern int material[WORKBENCH_TYPE_NUM + 1];

extern int buy[MATERIAL_TYPE_NUM + 1];
extern int sell[MATERIAL_TYPE_NUM + 1];

extern int workframe[WORKBENCH_TYPE_NUM + 1];

double factor(double x, double max_x, double min_rate);

int bitcount(int num);
void bitcount(int id, int num, std::list<int>* arr);

double th_unified(double dth);

double get_vth(double vel_x, double vel_y);
double get_linear(double vel_x, double vel_y);
double get_distance(double dx, double dy);

int estimate_time(double start_x, double start_y, double end_x, double end_y, double th);
