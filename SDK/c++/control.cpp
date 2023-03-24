#include "control.h"

#include <cmath>

#include "main.h"
#include "config.h"

static inline double limitApply(float num, float min, float max) {
    return (num < min ? min : (num > max ? max : num));
}

void PID_Type_Init(PID_type *pid, double kp, double ki, double kd, double oLimit)
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;

    pid->oLimit = oLimit;

    pid->prevError = 0.0;
    pid->integ = 0.0;
}

double PID_control(PID_type *pid, double target, double state, double dt)
{
    double output = 0.0f;

    pid->error = target - state;
    pid->integ += pid->error * dt;

    pid->deriv = (pid->error - pid->prevError) / dt;

    pid->outP = pid->kp * pid->error;
    pid->outI = pid->ki * pid->integ;
    pid->outD = pid->kd * pid->deriv;

    output = pid->outP + pid->outI + pid->outD;

    pid->prevError = pid->error;

    //输出限幅
    if (pid->oLimit != 0)
    {
        output = limitApply(output, -pid->oLimit, pid->oLimit);
    }

    // debug("target:%lf state:%lf P:%lf I:%lf D:%lf out:%lf\n", target, state, pid->outP, pid->outI, pid->outD, output);

    return output;
}
