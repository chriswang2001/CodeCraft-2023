#include "robot.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "config.h"
#include "main.h"

robot::robot() {
    target_ID = -1;
}

bool robot::read(const char* buffer, int id) {  
    ID = id;
    if(EOF != sscanf(buffer, "%d %d %lf %lf %lf %lf %lf %lf %lf %lf",
            &workbench_ID, &material_type, &time_factor, &collision_factor,
            &angular_vel, &linear_vel_x, &linear_vel_y, 
            &th, &x, &y)) {
                if(material_type) {
                    occupy[material_type]++;
                } else if(target_ID != -1){
                    occupy[target_Type]++;
                }
            return true;
        }
    
    return false;
}

void robot::show() {
    debug("robot[%d]: wb:%d material:%d factor%lf %lf vel:%lf %lf %lf th:%lf pos:%lf %lf target:%d type:%d\n", ID,
            workbench_ID, material_type, time_factor, collision_factor,
            angular_vel, linear_vel_x, linear_vel_y, 
            th, x, y, target_ID, target_Type);
}

double robot::get_dx() {
    if(target_ID == -1)
        return 0;
    
    return workbenches[target_ID].getx() - x;
}

double robot::get_dy() {
    if(target_ID == -1)
        return 0;
    
    return workbenches[target_ID].gety() - y;
}

double robot::get_dth() {
    if(target_ID == -1)
        return 0;
        
    double dth = atan2(get_dy(), get_dx()) - th;
    return th_unified(dth);
}

double robot::time_estimate(workbench& wb) {
    double dx = wb.getx() - x;
    double dy = wb.gety() - y;
    double d = sqrt(dx*dx + dy*dy);

    double dth = th_unified(atan2(dy, dx) - th);
    double time = d/MAX_LINEAR_VEL + fabs(dth)/MAX_ANGULAR_VEL;

    return time;
}

double robot::get_score(workbench& wb) {
    double money = 0.0;
    double time = 1.0;

    int wb_time = wb.getTime();
    int wb_type = wb.getType();
    
    int *arr = find[material_type];
    int n = WORKBENCH_TYPE_NUM;
    if(wb.checkRobot(material_type) || std::find(arr, arr + n, wb.getType()) == arr + n)
    {
        // debug("error in %s-%d\n", __func__, __LINE__);
        return 0;
    }

    if(0 == material_type) {
        if(need[wb_type] - occupy[wb_type] <= 0) {
            return 0;
        }

        if(wb.getProduct()) {
            wb_time = 0;
        } else if(wb_time < 0){
            return 0;
        }

        double road_time = time_estimate(wb);
        time = wb_time - road_time > 0 ? wb_time : road_time;
        money = profit[wb_type];
    } else {
        if(wb.checkMaterial(material_type)) {
        // debug("error in %s-%d\n", __func__, __LINE__);
            return 0;
        }
        time = time_estimate(wb);
        money = profit[target_Type] / wb.needMaterial() * factor(time, 9000, 0.8);
    }

    double score = money / time;
    // debug("error in %s-%d\n", __func__, __LINE__);
    return score;
}

void robot::plan() {
    if(-1 != target_ID) {
        return;
    }

    int max_score = 0;
    int max_score_id = -1;

    for(int i = 0; i < workbench_num; i++) {
        int score = get_score(workbenches[i]);
        // debug("score[%d]:%d ", i, score);
        if(score > max_score) {
            max_score = score;
            max_score_id = i;
        }
    }

    if(max_score_id >= 0) {
        set(max_score_id);
    } else {
        debug("error in %s-%d\n", __func__, __LINE__);
        // printf("destroy %d\n", ID);
    }
}

void robot::set(int target_id) {

    target_ID = target_id;
    target_Type = workbenches[target_ID].getType();

    workbenches[target_ID].setRobot(material_type);
}

void robot::control() {
    if(target_ID < 0)
        return;
    
    if(workbenches[target_ID].getID() == workbench_ID) {
        linear_set = 0;
        angular_set = 0;

        if(0 == material_type) {
            if(FRAME_MAX - frameID < FRAME_LIMIT)
                return;
                        
            printf("buy %d\n", ID);
            debug("buy %d\n", ID);
            if(!workbenches[target_ID].getProduct())
                return;

        } else {
            printf("sell %d\n", ID);
            debug("sell %d\n", ID);
            if(workbenches[target_ID].checkMaterial(material_type)) {
                debug("error in %s-%d\n", __func__, __LINE__);
                return;
            }

            workbenches[target_ID].setMaterial(material_type);
        }

        workbenches[target_ID].unsetRobot(material_type);
        target_ID = -1;
        return;
    }
    
    double dth = get_dth();
    debug("dth:%lf\n", dth);

    if(fabs(dth) < ANGULAR_LIMIT) {
        angular_set = 0;
        linear_set = MAX_LINEAR_VEL;
    } else{
        if(dth > 0) {
            angular_set = MAX_ANGULAR_VEL;
        } else {
            angular_set = -MAX_ANGULAR_VEL;
        }
        if(fabs(dth) < M_PI_4) {
            linear_set = MAX_LINEAR_VEL - 24 * fabs(dth)/ M_PI;
        } else {
            linear_set = 0;
        }
    }
}

void robot::print() {
    debug("robot[%d] linear:%lf angular:%lf\n", ID, linear_set, angular_set);
    printf("forward %d %lf\n", ID, linear_set);
    printf("rotate %d %lf\n", ID, angular_set);
}

void robot::loop() {
    plan();

    control();

    print();

    //debug();
}
