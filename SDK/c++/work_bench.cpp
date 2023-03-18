#include "work_bench.h"

#include <iostream>

#include "main.h"

work_bench::work_bench() {
    this->robot_ID = -1;
}

bool work_bench::read(const char* buffer, int ID) {
    this->ID = ID;

    if(EOF != sscanf(buffer, "%d %lf %lf %d %d %d", 
        &this->type, &this->x, &this->y, 
        &this->time_left, &this->material_state, &this->product_state)) {
            return true;
        }

    return false;
}

void work_bench::print() {
    debug("wb[%d] type:%d pos:%lf %lf time:%d material:%d product:%d robotid:%d\n", this->ID,
        this->type, this->x, this->y, 
        this->time_left, this->material_state, this->product_state, this->robot_ID);
}

double work_bench::getx() {
    return this->x;
}

double work_bench::gety() {
    return this->y;
}

int work_bench::getID() {
    return this->ID;
}

int work_bench::getType() {
    return this->type;
}

int work_bench::getTime() {
    return this->time_left;
}

int work_bench::getProduct() {
    return this->product_state;
}

bool work_bench::checkMaterial(int type) {
    return this->material_state & (1 << type);
}

void work_bench::setMaterial(int type) {
    this->material_state |= (1 << type);
}

int work_bench::getMaterial() {
    return this->material_state;
}

int work_bench::getRobotID() {
    return this->robot_ID;
}

void work_bench::setRobotID(int robot_ID) {
    this->robot_ID = robot_ID;
}
