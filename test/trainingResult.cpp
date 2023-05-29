#include <iostream>
#include <string>
#include <random>

#include <unistd.h> 			// POSIX 운영체제 API에 대한 액세스 제공
#include <fcntl.h> 			// 타겟시스템 입출력 장치 관련
#include <sys/types.h> 		// 시스템에서 사용하는 자료형 정보
#include <stdlib.h>
// #include<sys/ioctl.h> 		// 하드웨어의 제어와 상태 정보
#include <sys/stat.h> 		// 파일의 상태에 대한 정보
#include <time.h> 			// 시간 관련

#define fnd "/dev/fnd" 		// 7-Segment FND 
#define dot "/dev/dot" 		// Dot Matrix
#define tact "/dev/tactsw" 	// Tact Switch
#define led "/dev/led" 		// LED 
#define dip "/dev/dipsw"	// Dip Switch
#define clcd "/dev/clcd" 	// Character LCD

using namespace std;

int printClcd(string str);
int getTactSw(unsigned char& input);
int drawDotMTX(unsigned char& input, unsigned int sleepSec);
int drawDotMTX2(unsigned char input[9], unsigned int sleepSec);
int drawDotMTX3(unsigned char* input, unsigned int sleepSec);

int clcds;
int dipSw;
int fnds;
int tactSw;
int dotMtx;
int a = 0b0101100;
unsigned char figure[] = {0x44, 69, 70, 71, 72, 73, 74, 75}; //E
unsigned char figure2[] = {0x44, 69, 70, 0x7C, 0x60, 0x60, 0xff, 0xff};
unsigned char figure3[] = {0x44, 69, 70, 0x7C, 0x60, 0xff, 0xff, 0xff};
int arr[] = {1,2,3,4};

int running;
int flying;
int swimming;

/*
훈련 후 성공, 실패 와 능력치 어떻게 올릴 것인지 처리(
dip sw 올렸을 때의 작동
성공 실패 계산 후 능력치 상승까지
*/

void trainingResult(int successRate, vector<int> trainings){
    srand((unsigned int)time(NULL));
    int dice = random() % 100;
    if ( successRate <= dice ){ // 실패
        // printClcd("Training failed");
        // drawDotMTX(우는 이모티콘);
        cout << "Training failed: " << dice << endl;
        return;
    }
    // 성공
    int volume = 10;
    int pre_training = 0;
    vector<int> table(4, 0);
    for (int i=0; i < trainings.size(); i++) {
        int training = trainings[i];

        if (training > 4) {
            cout << training << ": invalid input" << endl;
            break;
        }
        else if (training == 4){
            // 사냥인 경우, 이전 훈련을 2번한 것과 같은 효과지만 뒤의 훈련의 효율이 떨어짐 (80%)
            table[pre_training] += volume * 2;
            volume = volume * 8 / 10;
            cout << "current volume : " << volume << endl;
        }
        else {
            table[training] += volume;
            pre_training = training;
        }
    }
    // printClcd("Training Success!");
    cout << "running : " << table[1] << endl;
    cout << "flying : " << table[2] << endl;
    cout << "swimming : " << table[3] << endl;
}

int main() {
    
    int successRate = 100;
    vector<int> test {4,1,2,3,4,1,4,5,2,3,2,1};
    trainingResult(50, test);


    return 0;
}

int printClcd(string str){
    clcds = open(clcd, O_RDWR);

    if (clcds < 0){
        cout << "can't find Dev dirver" << endl;
        return -1; 
    }

    if (write(clcds, str.c_str(), str.size()) == -1){
        cout << "file write error" << endl; // str.size()이 걸로 되는지 모르겠음
        return -1;
    } 
    close(clcds);
    return 0;
}

int getTactSw(unsigned char& input){
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
    cout << &input << endl;
    if (dotMtx < 0) {
        cout << "can't find Dev dirver" << endl;
        return -1; 
    }
    write(dotMtx, &input, 8);
    usleep(sleepSec);
    close(dotMtx);
    return 0;
}

int drawDotMTX2(unsigned char input[9], unsigned int sleepSec){
    dotMtx = open(dot, O_RDWR);
    cout << &input << endl;
    if (dotMtx < 0) {
        cout << "can't find Dev dirver" << endl;
        return -1; 
    }
    write(dotMtx, &input, 8);
    usleep(sleepSec);
    close(dotMtx);
    return 0;
}

int drawDotMTX3(unsigned char* input, unsigned int sleepSec){
    dotMtx = open(dot, O_RDWR);
    cout << &input << endl;
    if (dotMtx < 0) {
        cout << "can't find Dev dirver" << endl;
        return -1; 
    }

    write(dotMtx, &input, 8);
    usleep(sleepSec);
    close(dotMtx);
    return 0;
}
