#pragma once

#include <cmath>
#include <iostream>

#include "workbench.h"

class robot
{
private:
    int ID;
    int target_ID;
    int target_Type;
    double linear_set;
    double angular_set;

    int workbench_ID;
    int material_type;
    double time_factor;
    double collision_factor;
    double angular_vel;
    double linear_vel_x;
    double linear_vel_y;
    double th;
    double x;
    double y;

    double get_dx();
    double get_dy();
    double get_dth();

    double get_score(workbench& wb);
    double time_estimate(workbench& wb);
    
    void set(int target_id);

    void plan();
    void control();
public:
    robot();

    bool read(const char* buffer, int id);
    void show();

    void print();

    void loop();
};
