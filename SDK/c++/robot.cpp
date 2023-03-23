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

double robot::get_score(workbench& wb, int& next_id) {    
    int wb_time = wb.getTime();
    int wb_type = wb.getType();
        
    if(wb.checkRobot(material_type) || wb_type > 7 || wb_type <= 0 || need[wb_type].size() <= 0)
    {
        // debug("check:%d wb_type:%d need:%d", wb.checkRobot(material_type), wb_type, need[wb_type].size());
        return 0;
    }

    if(wb.getProduct()) {
        wb_time = 0;
    } else if(wb_time < 0){
        // debug("error in %s-%d\n", __func__, __LINE__);
        return 0;
    }

    int road_time = estimate_time(x, y, wb.getx(), wb.gety(), th);
    int buy_time = wb_time - road_time > 0 ? wb_time : road_time;

    double max_score = 0.0;

    std::list<int>& l = need[wb_type];
    for(auto it =l.begin(); it != l.end(); it++) {
        workbench& next_wb = workbenches[*it];

        int sell_time = estimate_time(wb.getx(), wb.gety(), next_wb.getx(), next_wb.gety(), atan2(wb.gety()-y, wb.getx()-x));
        double buy_profit =  sell[wb_type]*factor(sell_time, 9000, 0.8) - buy[wb_type];

        double sell_profit = 0.0;
        if(next_wb.getType() >=4 && next_wb.getType() <= 7) {
            sell_profit = (double)(sell[next_wb.getType()] - buy[next_wb.getType()]) / next_wb.needMaterial();
        }

        double score = (double)(buy_profit + sell_profit*0.5) / (buy_time + sell_time);

        debug("wb[%d]-wb[%d]'s socre:%lf buy time:%d money:%lf sell time:%d money:%lf\n", wb.getID(), *it, score, buy_time, buy_profit, sell_time, sell_profit);
        if(score > max_score) {
            max_score = score;
            next_id = *it;
        }
    }

    return max_score;
}

void robot::plan() {
    if(-1 != target_ID || material_type != 0) {
        // debug("error in %s-%d\n", __func__, __LINE__);
        return;
    }

    double max_score = 0;
    int max_score_id = -1;
    int max_next_id = -1;

    for(int i = 0; i < workbench_num; i++) {
        int next_id = -1;
        double score = get_score(workbenches[i], next_id);
        // debug("score[%d]:%lf\n", i, score);
        if(score > max_score) {
            max_score = score;
            max_score_id = i;
            max_next_id = next_id;
        }
    }

    if(max_score_id >= 0) {
        set(max_score_id, max_next_id);
    } else {
        debug("error in %s-%d\n", __func__, __LINE__);
    }
}

void robot::set(int target_id, int next_id) {

    target_ID = target_id;
    next_ID = next_id;
    
    debug("set: target-%d next-%d\n", target_ID, next_ID);
    target_Type = workbenches[target_ID].getType();
    workbenches[target_ID].setRobot(material_type);

    int size_before = need[target_Type].size();
    need[target_Type].remove(next_ID);
    int size_after = need[target_Type].size();

    if(size_before != size_after +1)
        debug("error in %s-%d\n", __func__, __LINE__);

    // workbench& wb = workbenches[target_ID];
    // workbench& next_wb = workbenches[next_ID];
    // road_time = estimate_time(x, y, wb.getx(), wb.gety(), th);
    // buy_time = wb.getTime() - road_time > 0 ? wb.getTime() : road_time;
    // sell_time = estimate_time(wb.getx(), wb.gety(), next_wb.getx(), next_wb.gety(), atan2(wb.gety()-y, wb.getx()-x));
    // set_time = frameID;
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
            
            // debug("robot[%d] road_time:%d actually:%d\n", ID, road_time, frameID-set_time);

            if(!workbenches[target_ID].getProduct()) {
                linear_set = MIN_LINEAR_VEL;
                return;
            }

            // if(abs(buy_time - frameID + set_time) > 10)
            //     debug("robot[%d] buy_time:%d actually:%d\n", ID, buy_time, frameID-set_time);
            printf("buy %d\n", ID);
            debug("buy %d\n", ID);

            workbenches[target_ID].unsetRobot(material_type);
            target_ID = next_ID;
            workbenches[target_ID].setRobot(target_Type);
            target_Type = workbenches[target_ID].getType();
        } else {
            printf("sell %d\n", ID);
            debug("sell %d\n", ID);

            if(workbenches[target_ID].checkMaterial(material_type)) {
                debug("error in %s-%d\n", __func__, __LINE__);
                return;
            }

            // if(abs(buy_time - frameID + set_time) > 10)
            //     debug("robot[%d] sell_time:%d actually:%d\n", ID, sell_time, frameID-set_time);

            if(9 == target_Type || 8 == target_Type) {
                bitcount(target_ID, 1 << material_type, need);
            }
            
            workbenches[target_ID].unsetRobot(material_type);
            target_ID = -1;
        }
    } else {
        double dth = get_dth();
        // debug("dth:%lf\n", dth);

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
}
