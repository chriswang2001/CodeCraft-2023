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

    bool checkMaterial(int m_type);
    bool checkNeedMaterial(int m_type);
    int needMaterial();

    void setRobot(int m_type);
    void unsetRobot(int m_type);
    bool checkRobot(int m_type);
};
