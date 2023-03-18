#pragma once

#include <cmath>
#include <iostream>

#include "work_bench.h"

class robot
{
private:
    int ID;
    int target_ID;
    int target_Type;
    double linear_set;
    double angular_set;

    int work_bench_ID;
    int material_type;
    double time_factor;
    double collision_factor;
    double angular_vel;
    double linear_vel_x;
    double linear_vel_y;
    double th;
    double x;
    double y;

    double score(work_bench& wb);
    void set(int target_ID);

public:
    robot();

    bool read(const char* buffer, int ID);
    void print();

    void loop();
    void plan();
    void control();
};
