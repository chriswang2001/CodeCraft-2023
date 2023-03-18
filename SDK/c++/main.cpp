#include "main.h"

#include <iostream>

#include "work_bench.h"
#include "robot.h"
#include "config.h"

using namespace std;

#ifdef DEBUG
FILE * fp;
#endif

int money, work_bench_num;

work_bench work_benches[WORK_BENCH_NUM_MAX];
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
            sscanf(line, "%d", &work_bench_num);
            // debug("worknum: %d\n", work_bench_num);
            flag = 3;
        } else if(3 == flag) {
            work_benches[count].read(line, count);
            work_benches[count].print();

            if(++count >= work_bench_num) {
                flag = 4;
                count = 0;
            }
        } else if(4 == flag) {
            robots[count].read(line, count);
            robots[count].print();

            if(++count >= ROBOT_NUM) {
                flag = 5;
                count = 0;
            }
        }

        // debug("%s\n", line);

    }

    return false;
}

int main() {
    #ifdef DEBUG
    fp = fopen ("debug.txt", "w");
    #endif
    readUntilOK(0);
    for(int i = 1; i < MATERIAL_TYPE_NUM + 1; i++) {
        debug("myneed[%d]:%d(%d-%d) ", i, need[i] - occupy[i], need[i], occupy[i]);
        occupy[i] = 0;
    }
    puts("OK");
    fflush(stdout);

    int frameID;

    while (scanf("%d", &frameID) != EOF) {
        debug("\nframe %d\n", frameID);
        readUntilOK(1);
        printf("%d\n", frameID);
        
        for(int robotId = 0; robotId < ROBOT_NUM; robotId++){
            robots[robotId].loop();
        }

        for(int i = 1; i < MATERIAL_TYPE_NUM + 1; i++) {
            debug("myneed[%d]:%d(%d-%d) ", i, need[i] - occupy[i], need[i], occupy[i]);
            occupy[i] = 0;
        }

        printf("OK\n");
        fflush(stdout);
    }

#ifdef DEBUG
    fclose(fp);
#endif

    return 0;
}
