#pragma once

class workbench
{
private:
    int ID;
    int robot_ID;

    int type;
    double x;
    double y;
    int time_left;
    int material_state;
    int product_state;

public:
    workbench();

    bool read(const char* buffer, int id);
    void print();

    double getx();
    double gety();
    int getID();
    int getType();
    int getTime();

    bool checkMaterial(int type);
    void setMaterial(int type);
    int getMaterial();

    int getProduct();

    int getRobotID();
    void setRobotID(int robot_id);
};
