#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double qTable[16][4];
int rewardTable[16][4];

double alpha = 0.2;
double gama = 0.95;
double epsilon = 1;

void printQTable() {
    printf("\tUP\t\t\tRIGHT\t\tLEFT\t\tDOWN\n");

    int i, j;
    for (i = 0; i < 16; i++) {
        printf("%d.\t", i);
        for (j = 0; j < 4; j++) {
            printf("%f\t", qTable[i][j]);
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

    int ar_direction[4] = {0, 0, 0, 0};
    int count = 0;
    for (direction = 0; direction < 4; direction++) {
        if (max == qTable[state][direction]) {
            ar_direction[direction] = -1;
            count++;
        }
    }

    int ar_random_direction[count];
    int index = 0;
    for (direction = 0; direction < 4; direction++) {
        if (ar_direction[direction] == -1) {
            ar_random_direction[index] = direction;
            index++;
        }
    }
    // srand(time(0));
    int select_index = (rand() % count);
    m = ar_random_direction[select_index];


    return m;
}

int qValueUpdate(int state, int direction) {
    int directionArray[4] = {-4, 1, -1, 4};

    int newState = state;
    int i = 0;
    for (i = 0; i < 4; i++) {
        if (i == direction) {
            newState = state + directionArray[i];
            break;
        }
    }

    if (newState > 15 || newState < 0 || (((state % 4) == 0) && direction == 2) ||
        (((state % 4) == 3) && direction == 1)) {
        qTable[state][direction] = qTable[state][direction] + alpha * (rewardTable[state][direction]
                                                                       + gama * maxQ(state) -
                                                                       qTable[state][direction]);
        return state;
    }

    qTable[state][direction] = qTable[state][direction] + alpha * (rewardTable[state][direction]
                                                                   + gama * maxQ(state + directionArray[direction]) -
                                                                   qTable[state][direction]);
    return state + directionArray[direction];

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

    int currentState = rand() % 16;
    int episode = 5000;
    while (episode > 0) {
        printf("current: %d\n", currentState);
        double random_value = randomDouble(0.0, 1.0);
        // double random_value = 0.05;
        if (random_value <= epsilon) {
            int direction = rand() % 4;
            currentState = qValueUpdate(currentState, direction);
        } else {
            int direction = maxDirection(currentState);
            currentState = qValueUpdate(currentState, direction);
        }

        if (currentState == 15) {
            printf("current: %d\n", currentState);
            currentState = rand() % 16;
            printf("G O A L ! ! !\n\n");
            episode--;
        }

        epsilon -= 1.0 / 5000;
    }
    printQTable();

    return 0;
}