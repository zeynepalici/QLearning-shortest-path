#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double qTable[16][4];
int rewardTable[16][4];

double alpha = 0.2;
double gama = 0.95;
double epsilon = 0.25;

void printQTable() {
    int i, j;
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 4; j++) {
            printf("%d )   %f ",i, qTable[i][j]);
        }
        printf("\n");
    }
}

double maxQ(int state) {
    int direction = 0;
    double max = 0;
    for (direction = 0; direction < 4; direction++) {
        if (qTable[state][direction] > max)
            max = qTable[state][direction];
    }
    return max;
}

int maxDirection(int state) {
    int direction;
    int m = 0;
    double max = 0;


    for (direction = 0; direction < 4; direction++) {
        if (qTable[state][direction] > max) {
            max = qTable[state][direction];
        }
    }

    int ar_direction[4] = {0,0,0,0};
    int count = 0;
    for(direction=0; direction<4; direction++){
        if (max == qTable[state][direction]){
            ar_direction[direction] = -1;
            count++;
        }
    }

    int ar_random_direction[count];
    int index = 0;
    for(direction = 0; direction<4; direction++){
        if(ar_direction[direction] == -1){
            ar_random_direction[index] = direction;
            index++;
        }
    }
   // srand(time(0));
    int select_index= (rand() % count);
    m = ar_random_direction[select_index];


    return m;
}

int qValueUpdate(int state, int direction) {
    switch (direction) {
        case 0://yukari

            if (state < 4) {
                qTable[state][direction] = qTable[state][direction] + alpha * (rewardTable[state][direction]
                                                                               + gama * maxQ(state) -
                                                                               qTable[state][direction]);
                return state;
            } else {
                qTable[state][direction] = qTable[state][direction] + alpha * (rewardTable[state][direction]
                                                                               + gama * maxQ(state - 4) -
                                                                               qTable[state][direction]);
                return state - 4;
            }

        case 1: //sag
            if (state == 3 || state == 7 || state == 11 || state == 15) {
                qTable[state][direction] = qTable[state][direction] + alpha * (rewardTable[state][direction]
                                                                               + gama * maxQ(state) -
                                                                               qTable[state][direction]);
                return state;
            } else {
                qTable[state][direction] = qTable[state][direction] + alpha * (rewardTable[state][direction]
                                                                               + gama * maxQ(state + 1) -
                                                                               qTable[state][direction]);
                return state + 1;
            }

        case 2: //sol
            if (state == 0 || state == 4 || state == 8 || state == 12) {
                qTable[state][direction] = qTable[state][direction] + alpha * (rewardTable[state][direction]
                                                                               + gama * maxQ(state) -
                                                                               qTable[state][direction]);
                return state;

            } else {
                qTable[state][direction] = qTable[state][direction] + alpha * (rewardTable[state][direction]
                                                                               + gama * maxQ(state - 1) -
                                                                               qTable[state][direction]);
                return state - 1;
            }
        case 3: //asagi
            if (state >= 12) {
                qTable[state][direction] = qTable[state][direction] + alpha * (rewardTable[state][direction]
                                                                               + gama * maxQ(state) -
                                                                               qTable[state][direction]);
                return state;
            } else {
                qTable[state][direction] = qTable[state][direction] + alpha * (rewardTable[state][direction]

                                                                               + gama * maxQ(state + 4) -
                                                                               qTable[state][direction]);

                return state + 4;
            }
    }
    printf("***********************");
    return state;
}

double randomDouble(double low, double high) {

    // srand(time(0));
    return ((double) rand() * (high - low)) / (double) RAND_MAX + low;
}

int main() {
    int i, j = 0;
    srand(time(0));

    for (i = 0; i < 16; i++) {
        for (j = 0; j < 4; j++) {
            qTable[i][j] = 0;
            rewardTable[i][j] = 0;
        }
    }

    rewardTable[14][1] = 100;
    rewardTable[11][3] = 100;

    int currentstate = rand()%16;
    //int direction=0;

    int episode = 100000;
    while (episode > 0) {
        printf("current: %d\n", currentstate);

        double random_value = randomDouble(0.0, 1.0);
        // double random_value = 0.05;
        if (random_value <= epsilon) {
            int direction = (rand() % 4);
            currentstate = qValueUpdate(currentstate, direction);
        } else {
            int direction = maxDirection(currentstate);
            currentstate = qValueUpdate(currentstate, direction);
        }
        if (currentstate == 15) {
            printf("GOAL!!!! \n");
            currentstate = rand()%16;
            //episode--;
            //printf("count: %d\n", count);
        }


        epsilon -= 0.00005;
        episode--;
    }
    printQTable();

    return 0;
}