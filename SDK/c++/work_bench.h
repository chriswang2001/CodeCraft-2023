#pragma once

class work_bench
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
    work_bench();

    bool read(const char* buffer, int ID);
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
    void setRobotID(int robot_ID);
};
