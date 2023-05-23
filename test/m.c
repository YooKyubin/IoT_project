#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define FND "/dev/fnd"
#define DOT "/dev/dot"
#define TACT "/dev/tactsw"
#define LED "/dev/led"
#define DIP "/dev/dipsw"
#define CLCD "/dev/clcd"

int printClcd(const char* str);
int getTactSw(unsigned char* input);
int drawDotMTX(unsigned char* input, unsigned int sleepSec);

int clcds;
int dipSw;
int fnds;
int tactSw;
int dotMtx;
unsigned char figure[8] = {0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x7E, 0x00};

int main() {
    printClcd("Press any key to start Game");

    while (1) {
        unsigned char tactSwInput = 0;
        getTactSw(&tactSwInput);
        if (tactSwInput == 0)
            continue;
        else {
            printf("Current tact switch input: %d\n", tactSwInput);
            break;
        }
    }

    printClcd("Start Program");

    clock_t startTime, currentTime;
    startTime = clock();
    currentTime = clock();
    while ((currentTime - startTime) < 5000) {
        drawDotMTX(figure, 450000);
        currentTime = clock();
    }

    printClcd("Press any key to terminate Program");

    while (1) {
        unsigned char tactSwInput = 0;
        getTactSw(&tactSwInput);
        if (tactSwInput == 0)
            continue;
        else {
            printf("Current tact switch input: %d\n", tactSwInput);
            break;
        }
    }

    return 0;
}

int printClcd(const char* str) {
    clcds = open(CLCD, O_RDWR);

    if (clcds < 0) {
        printf("Can't find Dev driver\n");
        return -1;
    }

    if (write(clcds, str, strlen(str)) == -1) {
        printf("File write error\n");
        return -1;
    }

    close(clcds);
    return 0;
}

int getTactSw(unsigned char* input) {
    tactSw = open(TACT, O_RDWR);
    if (tactSw < 0) {
        printf("Can't find Dev driver\n");
        return -1;
    }

    read(tactSw, input, sizeof(*input));
    close(tactSw);
    return 0;
}

int drawDotMTX(unsigned char* input, unsigned int sleepSec) {
    dotMtx = open(DOT, O_RDWR);
    if (dotMtx < 0) {
        printf("Can't find Dev driver\n");
        return -1;
    }

    write(dotMtx, input, sizeof(*input));
    usleep(sleepSec);
    close(dotMtx);
    return 0;
}