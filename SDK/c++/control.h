#pragma once

typedef struct //定义记录每个方向pid各量的结构
{
    double error;            //< error
    double prevError;        //< previous error
    double integ;            //< integral
    double deriv;            //< derivative
    double kp;               //< proportional gain
    double ki;               //< integral gain
    double kd;               //< derivative gain
    double outP;             //< proportional output (debugging)
    double outI;             //< integral output (debugging)
    double outD;             //< derivative output (debugging)
    double oLimit;           //< total PID output limit, absolute value. '0' means no limit.
} PID_type;

void PID_Type_Init(PID_type *pid, double kp, double ki, double kd, double oLimit);
double PID_control(PID_type *pid, double target, double state, double dt);
