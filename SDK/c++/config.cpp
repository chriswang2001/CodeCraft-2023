#include "config.h"

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
