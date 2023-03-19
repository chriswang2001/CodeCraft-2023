#include "workbench.h"

#include <iostream>

#include "main.h"

workbench::workbench() {
    robot_state = 0;
}

bool workbench::read(const char* buffer, int id) {
    ID = id;

    if(EOF != sscanf(buffer, "%d %lf %lf %d %d %d", 
        &type, &x, &y, 
        &time_left, &material_state, &product_state)) {
            bitcount(material_state, occupy);
            return true;
        }

    return false;
}

void workbench::print() {
    debug("wb[%d] type:%d pos:%lf %lf time:%d material:%d product:%d robot:%d\n", ID,
        type, x, y, 
        time_left, material_state, product_state, robot_state);
}

double workbench::getx() {
    return x;
}

double workbench::gety() {
    return y;
}

int workbench::getID() {
    return ID;
}

int workbench::getType() {
    return type;
}

int workbench::getTime() {
    return time_left;
}

int workbench::getProduct() {
    return product_state;
}

bool workbench::checkMaterial(int type) {
    return material_state & (1 << type);
}

void workbench::setMaterial(int type) {
    material_state |= (1 << type);
}

void workbench::setRobot(int type) {
    robot_state |= (1 << type);
}

void workbench::unsetRobot(int type) {
    robot_state &= ~(1 << type);
}

bool workbench::checkRobot(int type) {
    return robot_state & (1 << type);
}
