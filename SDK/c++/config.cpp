#include "config.h"

#include <cmath>

int material[WORKBENCH_TYPE_NUM + 1] = {0, 0, 0, 0, 0b00000110, 0b00001010, 0b00001100, 0b01110000, 0b10000000, 0b11111110};

int profit[MATERIAL_TYPE_NUM + 1] = {0, 6000-3000, 7600-4400, 9200-5800, 22500-15400, 25000-17200, 27500-19200, 105000-76000};

int find[MATERIAL_TYPE_NUM + 1][WORKBENCH_TYPE_NUM] = {
    {1,2,3,4,5,6,7},
    {4, 5, 9},
    {4, 6, 9},
    {5, 6, 9},
    {7, 9},
    {7, 9},
    {7, 9},
    {8, 9}
};

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

void bitcount(int num, int* arr) {
    int count = 0;

    while(num != 0)
    {
        if(num & 1)
            arr[count]++;
        
        num = num >> 1;
        count++;
    }
}

double th_unified(double dth) {
    if(dth < -M_PI)
        dth += 2 * M_PI;
    else if(dth > M_PI)
        dth -= 2 * M_PI;
    
    return dth;
}
