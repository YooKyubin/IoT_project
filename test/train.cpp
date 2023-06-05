#include <iostream>
#include <vector>
#include <string>
#include <random>

#include <string.h>
#include <unistd.h> 			// POSIX 운영체제 API에 대한 액세스 제공
#include <fcntl.h> 			// 타겟시스템 입출력 장치 관련
#include <sys/types.h> 		// 시스템에서 사용하는 자료형 정보
// #include <sys/ioctl.h> 		// 하드웨어의 제어와 상태 정보
#include <sys/stat.h> 		// 파일의 상태에 대한 정보
#include <time.h> 			// 시간 관련
#include <stdlib.h>

#define fnd "/dev/fnd" 		// 7-Segment FND 
#define dot "/dev/dot" 		// Dot Matrix
#define tact "/dev/tactsw" 	// Tact Switch
#define led "/dev/led" 		// LED 
#define dip "/dev/dipsw"	// Dip Switch
#define clcd "/dev/clcd" 	// Character LCD

using namespace std;

unsigned char fnd_number[] = {~0x3f, ~0x06, ~0x5b, ~0x4f, ~0x66, ~0x6d, ~0x7d, ~0x07, ~0x7f, ~0x67, ~0x00};


void train(int& successRate, vector<int>& trainings);
int printFnd(int input, unsigned int sleepSec);
int getTactSw(int& input);
int drawDotMTX(unsigned char& input, unsigned int sleepSec);
int drawDotMTX(vector<unsigned char>& input, unsigned int sleepSec);
int printClcd(string str);

int fnds;
int clcds;
int dipSw;
int tactSw;
int dotMtx;

vector<unsigned char> face {0x00, 0x3c, 0x7e, 0x5a, 0x66, 0x7e, 0x66, 0x42};

int main() {
    vector<int> trainings;
    int successRate = 100;

    for(int i=0; i < 4; i++) {
        train(successRate, trainings);
    }
    return 0;
}

// 현재 예약된 훈련 목록을 clcd로 보여줌, dotmtx로 현재 얼굴 보여줌, FND로 현재 성공확률 보여줌
void train(int& successRate, vector<int>& trainings){
    // if ( 돌봐주기 성공 ) { successRate += 10; }
    int training = 0;
    string trainClcd = "Today's traingin : ";
    printClcd(trainClcd);
    
    drawDotMTX(face, 250000); // 0.25s
    while (training == 0){
        getTactSw(training);
        printFnd(successRate, 250000); // 0.25s
    }

    if ( training > 4 ){
        //잘못된 입력
    }
    else{
        trainings.push_back(training);
        trainClcd += to_string(training);
        successRate -= 10;
        if (training == 4){
            successRate += 3; // 사냥은 7 감소
        }
    } 
    return;
}

int printClcd(string str){
    clcds = open(clcd, O_RDWR);

    if (clcds < 0){
        cout << "can't find Dev dirver" << endl;
        return -1; 
    }

    if (write(clcds, str.c_str(), 32) == -1){
        cout << "file write error" << endl;
        return -1;
    } 
    close(clcds);
    return 0;
}

int printFnd(int input, unsigned int sleepSec){

    vector<unsigned char> data(4, fnd_number[0]);
    data[0] = fnd_number[ input / 1000  % 10 ];
    data[1] = fnd_number[ input / 100   % 10 ];
    data[2] = fnd_number[ input / 10    % 10 ];
    data[3] = fnd_number[ input / 1     % 10 ];

    fnds=open(fnd, O_RDWR);
    if (fnds < 0) {
        cout << "can't find Dev dirver" << endl;
        return -1; 
    }
    write(fnds, data.data(), 4);
    usleep(sleepSec);
    close(fnds);
    return 0;
}

int getTactSw(int& input){
    tactSw = open(tact, O_RDWR);
    if (tactSw < 0) {
        cout << "can't find Dev driver" << endl;
        return -1;
    }
    read(tactSw, &input, sizeof(input));
    close(tactSw);
    return 0;
}

int drawDotMTX(unsigned char& input, unsigned int sleepSec){
    dotMtx = open(dot, O_RDWR);
    if (dotMtx < 0) {
        cout << "can't find Dev dirver" << endl;
        return -1; 
    }
    write(dotMtx, &input, 8);
    usleep(sleepSec);
    close(dotMtx);
    return 0;
}

int drawDotMTX(vector<unsigned char>& input, unsigned int sleepSec){
    dotMtx = open(dot, O_RDWR);
    if (dotMtx < 0) {
        cout << "can't find Dev dirver" << endl;
        return -1; 
    }
    write(dotMtx, input.data(), 8);
    usleep(sleepSec);
    close(dotMtx);
    return 0;
}