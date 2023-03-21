#include "main.h"

#include <iostream>

#include "workbench.h"
#include "robot.h"
#include "config.h"

using namespace std;

#ifdef DEBUG
FILE * fp;
#endif

int frameID, preFrameID;

int money, workbench_num;

workbench workbenches[WORKBENCH_NUM_MAX];
robot robots[ROBOT_NUM];

int need[MATERIAL_TYPE_NUM + 1];
int occupy[MATERIAL_TYPE_NUM + 1];

// 0代表读地图 1代表读每一帧的控制信息
bool readUntilOK(int flag) {
    char line[1024];
    int count = 0;
    while (fgets(line, sizeof line, stdin)) {
        if (line[0] == 'O' && line[1] == 'K') {
            return true;
        }
        
        if(0 == flag) {
            for(int i = 0; i < MAP_INPUT_LEN; i++) {
                if(line[i] > '0' && line[i] <='9')
                    bitcount(material[line[i]- '0'], need);
            }
        } else if(1 == flag) {
            sscanf(line, "%d", &money);
            // debug("money: %ld\n", money);
            flag = 2;
        } else if(2 == flag) {
            sscanf(line, "%d", &workbench_num);
            // debug("worknum: %d\n", workbench_num);
            flag = 3;
        } else if(3 == flag) {
            workbenches[count].read(line, count);
            workbenches[count].show();

            if(++count >= workbench_num) {
                flag = 4;
                count = 0;
            }
        } else if(4 == flag) {
            robots[count].read(line, count);
            robots[count].show();

            if(++count >= ROBOT_NUM) {
                flag = 5;
                count = 0;
            }
        }

        // debug("%s\n", line);

    }

    return false;
}

void printUtilOK() {
        printf("%d\n", frameID);
        
        for(int robotId = 0; robotId < ROBOT_NUM; robotId++) {
            robots[robotId].loop();
        }

        printf("OK\n");

        fflush(stdout);
}

int main() {
    #ifdef DEBUG
    fp = fopen ("debug.txt", "w");
    #endif
    readUntilOK(0);

    puts("OK");
    fflush(stdout);

    while (scanf("%d", &frameID) != EOF) {
        debug("\nframe %d\n", frameID);
        
        if(frameID != preFrameID + 1) {
            debug("error in %s-%d\n", __func__, __LINE__);
        }
        preFrameID = frameID;

        for(int i = 1; i < MATERIAL_TYPE_NUM + 1; i++) {
            occupy[i] = 0;
        }

        readUntilOK(1);

        printUtilOK();
    }

#ifdef DEBUG
    fclose(fp);
#endif

    return 0;
}
