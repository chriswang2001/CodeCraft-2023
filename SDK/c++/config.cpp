#include "config.h"

#include <cmath>

int material[WORKBENCH_TYPE_NUM + 1] = {0, 0, 0, 0, 0b00000110, 0b00001010, 0b00001100, 0b01110000, 0b10000000, 0b11111110};

int buy[MATERIAL_TYPE_NUM + 1] = {0, 3000, 4400, 5800, 15400, 17200, 19200, 76000};
int sell[MATERIAL_TYPE_NUM + 1] = {0, 6000, 7600, 9200, 22500, 25000, 27500, 105000};

int workframe[WORKBENCH_TYPE_NUM + 1] = {0, 50, 50, 50, 500, 500, 500, 1000, 1, 1};


double factor(double x, double max_x, double min_rate) {
    if(x >= max_x) {
        return min_rate;
    }

    double temp_a = 1 -  x/max_x;
    double temp_b = sqrt(1- temp_a * temp_a);

    return (1- temp_b)*(1-min_rate) + min_rate;
}

int bitcount(int num) {
    int count = 0;

    while(num != 0)
    {
        if(num & 1)
            count++;
        
        num = num >> 1;
    }

    return count;
}

void bitcount(int id, int num, std::list<int>* arr) {
    // debug("bicount %d num:%d\n", id, num);
    int count = 0;

    while(num != 0)
    {
        if(num & 1)
            arr[count].push_back(id);
        
        num = num >> 1;
        count++;
    }
}

double th_unified(double dth) {
    while(dth < -M_PI || dth > M_PI) {
        if(dth < -M_PI)
            dth += 2 * M_PI;
        else if(dth > M_PI)
            dth -= 2 * M_PI;
    }

    return dth;
}

double get_vth(double vel_x, double vel_y) {
    return atan2(vel_y, vel_x);
}

double get_linear(double vel_x, double vel_y) {
    return sqrt(vel_x*vel_x + vel_y*vel_y);
}

double get_distance(double dx, double dy) {
    return sqrt(dx*dx + dy*dy);
}

int estimate_time(double start_x, double start_y, double end_x, double end_y, double th) {
    double dx = end_x - start_x;
    double dy = end_y - start_y;
    double d = get_distance(dx, dy);

    double dth = th_unified(atan2(dy, dx) - th);

    double time = d / MAX_LINEAR_VEL + fabs(dth) / MAX_ANGULAR_VEL;

    return time * FRAME_PER_SECOND;
}
