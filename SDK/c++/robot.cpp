#include "robot.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "control.h"
#include "config.h"
#include "main.h"

robot::robot() {
    target_ID = -1;
    PID_Type_Init(&angular_PID, ANGULAR_PID_P, ANGULAR_PID_I, ANGULAR_PID_D, MAX_ANGULAR_VEL);
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
    debug("robot[%d]: wb:%d material:%d factor%lf %lf vel:%lf %lf %lf th:%lf pos:%lf %lf target:%d next:%d type:%d\n", ID,
            workbench_ID, material_type, time_factor, collision_factor,
            angular_vel, linear_vel_x, linear_vel_y, 
            th, x, y, target_ID, next_ID,target_Type);
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

double robot::get_vel_x() {
    return get_linear(linear_vel_x, linear_vel_y) * cos(th);
}

double robot::get_vel_y() {
    return get_linear(linear_vel_x, linear_vel_y) * sin(th);
}

double robot::get_radius() {
    if(0 == material_type) {
        return ROBOT_RADIUS_WITHOUT_MATERIAL;
    }

    return ROBOT_RADIUS_WITH_MATERIAL;
}

double robot::get_linear_acc() {
    double radius = get_radius();
    return MAX_TRACTION / (DENSITY * M_PI * pow(radius, 2.0));
}

double robot::get_angular_acc() {
    double radius = get_radius();
    return MAX_MONMENT / (DENSITY * M_PI * 0.5 * pow(radius, 4.0));
}

double robot::score_target(workbench& wb, int& next_id) {    
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
    if(wb_time > road_time + 10) {
        return 0;
    }
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
            sell_profit *= need[next_wb.getType()].size()/2.0;
        }

        double score = (double)(buy_profit + sell_profit) / (buy_time + sell_time);

        if(frameID + buy_time + sell_time + FRAME_LIMIT > FRAME_MAX) {
            score = 0;
        }

        // debug("wb[%d]-wb[%d]'s socre:%lf buy time:%d money:%lf sell time:%d money:%lf\n", wb.getID(), *it, score, buy_time, buy_profit, sell_time, sell_profit);
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
        double score = score_target(workbenches[i], next_id);
        // debug("score[%d]:%lf\n", i, score);
        if(score > max_score) {
            max_score = score;
            max_score_id = i;
            max_next_id = next_id;
        }
    }

    if(max_score_id >= 0) {
        set_target(max_score_id, max_next_id);
    } else {
        debug("error in %s-%d\n", __func__, __LINE__);
        for(int i = 1; i < MATERIAL_TYPE_NUM + 1; i++) {
            debug("need[%d]:%lu ", i, need[i].size());
        }
        debug("\n");
    }
}

void robot::set_target(int target_id, int next_id) {

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

    workbench& wb = workbenches[target_ID];
    workbench& next_wb = workbenches[next_ID];
    int road_frame = estimate_time(x, y, wb.getx(), wb.gety(), th);
    int wb_frame = wb.getTime();
    if(wb.getProduct()) {
        wb_frame = 0;
    }
    buy_frame = wb_frame - road_frame > 0 ? wb_frame : road_frame;
    sell_frame = estimate_time(wb.getx(), wb.gety(), next_wb.getx(), next_wb.gety(), atan2(wb.gety()-y, wb.getx()-x)) + buy_frame;
    set_frame = frameID;
}

bool robot::check_collision(robot& b, double linear, double vth) {
    double dx = b.x - x;
    double dy = b.y - y;

    double d = get_distance(dx, dy);
    double th = atan2(dy, dx);

    if(d >= DISTANCE_LIMIT) {
        return false;
    } else if(d < get_radius() + b.get_radius() - ROBOT_RADIUS_ERROR) {
        // target_v = MIN_ANGULAR_VEL;
        return false;
    }

    double dth = asin( (get_radius() + b.get_radius() - ROBOT_RADIUS_ERROR) / d);

    double vel_x = linear * cos(vth);
    double vel_y = linear * sin(vth);

    double dv_th = atan2(vel_y - b.get_vel_y(), vel_x - b.get_vel_x());

    if(dv_th < th - dth || dv_th > th + dth) {
        return false;
    }
    
    return true;
}

bool robot::check_collision(double linear, double vth) {
    for(int robotId = 0; robotId < ROBOT_NUM; robotId++) {
        if(robotId != ID) {
            if(check_collision(robots[robotId], linear, vth)) {
                // debug("collision check bewteen:%d-%d vel:%lf th:%lf\n", ID, robotId, get_linear(linear_vel_x, linear_vel_y), get_vth(linear_vel_x, linear_vel_y));
                return true;
            }
        }
    }

    return false;
}

//碰撞避免 使用velocity obstacle算法
void robot::avoid_collision() {
    double raw_v = get_linear(linear_vel_x, linear_vel_y);
    double raw_v_th = th;

    if(!check_collision(raw_v, raw_v_th)) {
        return;
    }

    double linear_acc = get_linear_acc();

    double min_v = raw_v;
    double min_v_th = raw_v_th;

    double max_score = 0.0;
    for(double v = 0.0; v <= MAX_LINEAR_VEL; v += 0.5)
        for(double v_th = -M_PI; v_th <= M_PI; v_th += 0.1) {
            if(!check_collision(v, v_th)) {
                double dv_time = fabs(raw_v - v) / linear_acc;
                double dth_time = fabs(th_unified(raw_v_th - v_th)) / MAX_ANGULAR_VEL;
                double score = 1 /(dv_time + dth_time);
                if(score > max_score) {
                    max_score = score;
                    min_v = v;
                    min_v_th = v_th;
                }
            }
        }

    target_v = min_v;
    target_th = min_v_th;
    debug("robot%d to avoid collision set v:%lf v_th:%lf\n", ID, target_v, target_th);
}

void robot::avoid_border() {
    if(x < BORDER_LIMIT && y < BORDER_LIMIT) {
        if(th < 0 || th > M_PI_2) {
            target_v = 0.0;
            target_th = M_PI_4;
        } else{
            target_v = MAX_LINEAR_VEL;
        }
    } else if(x < BORDER_LIMIT && y > MAP_MAX_LEN - BORDER_LIMIT) {
        if(th > 0 || th < -M_PI_2) {
            target_v = 0.0;
            target_th = -M_PI_4;
        } else{
            target_v = MAX_LINEAR_VEL;
        }
    } else if(x > MAP_MAX_LEN - BORDER_LIMIT && y < BORDER_LIMIT) {
        if(th  < M_PI_2) {
            target_v = 0.0;
            target_th = 3.0 * M_PI_4;
        } else{
            target_v = MAX_LINEAR_VEL;
        }
    } else if(x > MAP_MAX_LEN - BORDER_LIMIT && y > MAP_MAX_LEN - BORDER_LIMIT) {
        if(th > -M_PI_2) {
            target_v = 0.0;
            target_th = -3.0 *M_PI_4;
        } else{
            target_v = MAX_LINEAR_VEL;
        }
    }else if(x < BORDER_LIMIT) {
        if(th > M_PI_2 || th < -M_PI_2) {
            target_v = 0.0;
            target_th = 0.0;
        } else {
            target_v = MAX_LINEAR_VEL;
        }
    } else if(x > MAP_MAX_LEN - BORDER_LIMIT) {
        if(th < M_PI_2 && th > -M_PI_2) {
            target_v = 0.0;
            target_th = M_PI;
        } else {
            target_v = MAX_LINEAR_VEL;
        }
    }else if(y < BORDER_LIMIT) {
        if(th < 0) {
            target_v = 0.0;
            target_th = M_PI_2;
        } else {
            target_v = MAX_LINEAR_VEL;
        }
    } else if(y > MAP_MAX_LEN - BORDER_LIMIT) {
        if(th > 0) {
            target_v = 0.0;
            target_th = -M_PI_2;
        } else {
            target_v = MAX_LINEAR_VEL;
        }
    }
}

void robot::control() {
    if(target_ID < 0) {
        target_v = 0.0;
        target_th = th;

        goto end;
    }

    double dth;

    if(workbenches[target_ID].getID() == workbench_ID) {
        if(0 == material_type) {
            // debug("robot[%d] road_time:%d actually:%d\n", ID, road_frame, frameID-set_frame);

            if(!workbenches[target_ID].getProduct()) {
                target_v = 0.0;
                target_th = th;

                goto end;
            } else {
                if(abs(buy_frame - frameID + set_frame) > 10)
                    debug("robot[%d] buy_time:%d actually:%d\n", ID, buy_frame, frameID-set_frame);
                
                printf("buy %d\n", ID);
                debug("buy %d\n", ID);

                workbenches[target_ID].unsetRobot(material_type);
                target_ID = next_ID;
                workbenches[target_ID].setRobot(target_Type);
                target_Type = workbenches[target_ID].getType();
            }
        } else {
            if(workbenches[target_ID].checkMaterial(material_type)) {
                debug("error in %s-%d\n", __func__, __LINE__);
                printf("destroy %d\n", ID);
            } else {
                printf("sell %d\n", ID);
                debug("sell %d\n", ID);

                if(abs(sell_frame - frameID + set_frame) > 50)
                    debug("robot[%d] sell_time:%d actually:%d\n", ID, sell_frame, frameID-set_frame);

                if(9 == target_Type || 8 == target_Type) {
                    bitcount(target_ID, 1 << material_type, need);
                }
            }

            workbenches[target_ID].unsetRobot(material_type);
            target_ID = -1;

            goto end;
        }
    }

    dth = get_dth();
    target_th = th + dth;

    if(fabs(dth) < ANGULAR_LIMIT) {
        target_v = MAX_LINEAR_VEL;
    } else if(fabs(dth) < M_PI_4) {
        target_v = MAX_LINEAR_VEL - 24 * fabs(dth)/ M_PI;
    } else {
        target_v = 0;
    }

end:
    avoid_border();
    avoid_collision();

    linear_set = target_v;
    angular_set = PID_control(&angular_PID, target_th, th, (frameID - preFrameID) * FRAME_TIME);
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
