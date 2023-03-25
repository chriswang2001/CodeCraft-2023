#pragma once

class workbench
{
private:
    int ID;
    int robot_state;

    int type;
    double x;
    double y;
    int time_left;
    int material_state;
    int product_state;

public:
    workbench();

    bool read(const char* buffer, int id);
    void show();

    double getx();
    double gety();
    int getID();
    int getType();
    int getTime();
    int getProduct();

    bool checkMaterial(int type);
    bool checkNeedMaterial(int type);
    int needMaterial();

    void setRobot(int type);
    void unsetRobot(int type);
    bool checkRobot(int type);
};
