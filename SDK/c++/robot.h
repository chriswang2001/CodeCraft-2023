#pragma once

#include <cmath>
#include <iostream>

#include "control.h"
#include "workbench.h"

class robot
{
public:
    robot();

    bool read(const char* buffer, int id);
    void show();

    void print();
    void loop();

private:
    int ID;
    int target_ID;
    int next_ID;
    int target_Type;
    double linear_set;
    double angular_set;

    double target_th;
    double target_v;

    PID_type angular_PID;

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

    int set_frame;
    int road_frame;
    int buy_frame;
    int sell_frame;
    
    double get_dx();
    double get_dy();
    double get_dth();

    double get_vel_x();
    double get_vel_y();

    double get_radius();
    double get_linear_acc();
    double get_angular_acc();
    
    double score_target(workbench& wb, int& next_id);
    void set_target(int target_id, int next_id);

    bool check_collision(robot &b, double linear, double vth);
    bool check_collision(double linear, double vth);
    void avoid_collision();
    void avoid_border();

    void plan();
    void control();


};
