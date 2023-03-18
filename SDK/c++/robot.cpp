#include "robot.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "config.h"
#include "main.h"

int profit[MATERIAL_TYPE_NUM] = {3000, 3200, 3400, 7100, 7800, 8300, 29000};

int find0[MATERIAL_TYPE_NUM] ={1,2,3,4,5,6,7};

int find1[MATERIAL_TYPE_NUM][3] = {
    {4, 5, 9},
    {4, 6, 9},
    {5, 6, 9},
    {7, 9,-1},
    {7, 9,-1},
    {7, 9,-1},
    {8, 9,-1}
};

robot::robot() {
    this->target_ID = -1;
}

bool robot::read(const char* buffer, int ID) {  
    this->ID = ID;
    if(EOF != sscanf(buffer, "%d %d %lf %lf %lf %lf %lf %lf %lf %lf",
            &this->work_bench_ID, &this->material_type, &this->time_factor, &this->collision_factor,
            &this->angular_vel, &this->linear_vel_x, &this->linear_vel_y, 
            &this->th, &this->x, &this->y)) {
            return true;
        }

    return false;
}

void robot::print() {
    debug("robot[%d]: wb:%d material:%d factor%lf %lf vel:%lf %lf %lf th:%lf pos:%lf %lf target:%d type:%d\n", this->ID,
            this->work_bench_ID, this->material_type, this->time_factor, this->collision_factor,
            this->angular_vel, this->linear_vel_x, this->linear_vel_y, 
            this->th, this->x, this->y, this->target_ID, this->target_Type);
}

double robot::score(work_bench& wb) {

    int* arr, n;
    int score = 0;
    int time = wb.getTime();
    int type = wb.getType();
    
    //     debug("%s %d\n", __func__, __LINE__);

    if(0 == this->material_type) {
        if(time < 0 || need[type-1] <= 0) {
        // debug("%s %d\n", __func__, __LINE__);
            return 0;
        }

        if(time == 0)
            score += 3000;
        else{
            score += 3000/time;
        }
        score += profit[wb.getType()-1] * 1.5f;
        arr = find0;
        n = MATERIAL_TYPE_NUM;
    } else {
        if(wb.checkMaterial(this->material_type)) {
            return 0;
        }
        arr = find1[this->material_type-1];
        n = 3;
    }

    if(wb.getRobotID() != -1 || std::find(arr, arr + n, wb.getType()) == arr + n)
    {
        // debug("%d %d %d", arr[0], n, std::find(arr, arr + n, wb.getType()));
        // debug("%s %d\n", __func__, __LINE__);
        return 0;
    }

    double dx = wb.getx() - this->x;
    double dy = wb.gety() - this->y;
    double d2 = dx*dx + dy*dy;

    score += MAP_MAX_LEN * MAP_MAX_LEN / d2;
    //     debug("%s %d\n", __func__, __LINE__);

    return score;
}

void robot::plan() {
    int max_score = 0;
    int max_score_id = -1;

    //     debug("%s %d\n", __func__, __LINE__);

    for(int i = 0; i < work_bench_num; i++) {
        int score = this->score(work_benches[i]);
        // debug("score[%d]:%d\n", i, score);
        // work_benches[i].print();
        if(score > max_score) {
            max_score = score;
            max_score_id = i;
        }
    }

    if(max_score_id >= 0) {
        this->set(max_score_id);
    } else {
        // debug("", );
        // printf("destroy %d\n", this->ID);
    }

    //     debug("%s %d\n", __func__, __LINE__);
}

void robot::set(int target_ID) {
    //     debug("%s %d\n", __func__, __LINE__);

    this->target_ID = target_ID;
    this->target_Type = work_benches[target_ID].getType();

    work_benches[target_ID].setRobotID(this->ID);

    if(0 == this->material_type) {
        need[this->target_Type - 1]--;
    }

    //     debug("%s %d\n", __func__, __LINE__);
}

void robot::control() {
    double dx, dy, dth;
    //     debug("%s %d\n", __func__, __LINE__);

    if(this->target_ID < 0)
        return;
    
    if(work_benches[target_ID].getID() == this->work_bench_ID) {
        this->linear_set = 0;
        this->angular_set = 0;

        if(0 == this->material_type) {
            printf("buy %d\n", this->ID);
            debug("buy %d\n", this->ID);
            if(!work_benches[target_ID].getProduct())
                goto end;
            
            need_switch(this->target_Type);
        } else {
            printf("sell %d\n", this->ID);
            debug("sell %d\n", this->ID);
            if(!work_benches[target_ID].checkMaterial(this->material_type)) {
                work_benches[target_ID].setMaterial(this->material_type);
                if(this->target_Type == 8 || (this->target_Type == 9 && work_benches[target_ID].getMaterial() == 0x11111110) ){
                    need_switch(this->target_Type);
                }
            }
        }

        work_benches[target_ID].setRobotID(-1);
        this->target_ID = -1;
        goto end;
    }
    
    dx = work_benches[target_ID].getx() - x;
    dy = work_benches[target_ID].gety()- y;
    dth = atan2f(dy, dx) - this->th;
    if(dth < -M_PI)
        dth += 2 * M_PI;
    else if(dth > M_PI)
        dth -= 2 * M_PI;
    
    if(fabsf(dth) < ANGULAR_LIMIT) {
        this->angular_set = 0;
        this->linear_set = MAX_LINEAR_VEL;
    } else{
        if(dth > 0) {
            this->angular_set = MAX_ANGULAR_VEL;
        } else {
            this->angular_set = -MAX_ANGULAR_VEL;
        }

        this->linear_set = 0;
    }

end:
    // debug("robot:%d set:%d\t", this->ID, this->target_ID);
    // debug("dx:%lf dy:%lf dth:%lf atan2:%lf\t", dx, dy, dth, atan2f(dy, dx));
    // debug("control: %lf %lf\n", this->linear_set, this->angular_set);

    printf("forward %d %lf\n", this->ID, this->linear_set);
    printf("rotate %d %lf\n", this->ID, this->angular_set);

    //     debug("%s %d\n", __func__, __LINE__);
}


void robot::loop() {
    //     debug("%s %d\n", __func__, __LINE__);

    if(-1 == this->target_ID) {
        this->plan();
    }


    this->control();

    this->print();

    //     debug("%s %d\n", __func__, __LINE__);
}
