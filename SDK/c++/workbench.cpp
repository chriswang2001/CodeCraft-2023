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
            if(time_left == workframe[type] - 1) {
                bitcount(ID, material[type], need);
            }

            if(7 == type) {
                double want_add = 0.0;
                if(1 == needMaterial()) {
                    want_add = 0.25;
                } else if(2 == needMaterial()) {
                    want_add = 0.15;
                }

                for(int i = 4; i <=6; i++) {
                    if(checkNeedMaterial(i)) {
                        debug("want_add %d:%lf\n", i, want_add);
                        want[i] += want_add;
                    }
                }
            }
            
            return true;
        }

    return false;
}

void workbench::show() {
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

bool workbench::checkMaterial(int m_type) {
    return material_state & (1 << m_type);
}

bool workbench::checkNeedMaterial(int m_type) {
    int temp = robot_state & ~1;
    temp = material[type] - material_state - temp;
    return temp & (1 << m_type);
}

int workbench::needMaterial() {
    int temp = robot_state & ~1;
    return bitcount(material[type] - material_state - temp);
}

void workbench::setRobot(int m_type) {
    robot_state |= (1 << m_type);
}

void workbench::unsetRobot(int m_type) {
    robot_state &= ~(1 << m_type);
}

bool workbench::checkRobot(int m_type) {
    return robot_state & (1 << m_type);
}
