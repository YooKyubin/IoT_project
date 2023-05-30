#include <iostream>
#include <vector>
#include <string>
#include <random>

#include <string.h>
#include <unistd.h> 			// POSIX 운영체제 API에 대한 액세스 제공
#include <fcntl.h> 			// 타겟시스템 입출력 장치 관련
#include <sys/types.h> 		// 시스템에서 사용하는 자료형 정보
#include <sys/ioctl.h> 		// 하드웨어의 제어와 상태 정보
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

vector<int> trainingResult(int successRate, vector<int> trainings);

int getDipSw(unsigned char& input);
int clearClcd();
int printClcd(string str);
int getTactSw(int& input);
int drawDotMTX(unsigned char& input, unsigned int sleepSec);
int drawDotMTX(vector<unsigned char>& input, unsigned int sleepSec);

int clcds;
int dipSw;
int fnds;
int tactSw;
int dotMtx;
unsigned char egg[8] = {0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x7E, 0x00};
unsigned char immature[8] = {0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00};
unsigned char smile[8] = {0x00, 0x00, 0x42, 0xA5, 0x00, 0x00, 0x00, 0x00};
unsigned char TT[8] = {0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x00, 0x00};
unsigned char figure[] = {0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x7E, 0xff}; //E
unsigned char figure2[] = {0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0xff, 0xff};
unsigned char figure3[] = {0x7E, 0x60, 0x60, 0x7C, 0x60, 0xff, 0xff, 0xff};

struct Creature{
    int status;
    string name;
    // unsigned char face[8];
    vector<unsigned char> face;
    vector<int> state;

    void init(int current){
        status = current;
        state.assign(4,0);

        if (status == 0){
            name = "egg";
            // memcpy(&face, &egg, 8);
            face = {0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x7E, 0x00};
        }
        else{
            name = "immature";
            // memcpy(&face, &immature, 8);
            face = {0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00};
        }
    }
};

int main() {
    
    printClcd("Press any key to start Game");
    while (true){
        int tactSwInput = 0;
        getTactSw(tactSwInput);
        if (tactSwInput == 0) continue;
        else{
            cout << "Current tact switch input : " << tactSwInput << endl;
            break;
        }
    }
    clearClcd();

    Creature creature;

    // Game start
    for (int status=0; status < 2; status++){
        creature.init(status);
        drawDotMTX(creature.face, 2000000);
        
        int day = 0;
        unsigned char dipSwInput;
        getDipSw(dipSwInput);
        if (dipSwInput != 0){
            printClcd("Please init  dip switch ");
            while(dipSwInput != 0){
                getDipSw(dipSwInput);
            }
            clearClcd();
        }

        while (dipSwInput < 255){
            unsigned char pre_dipSwInput = dipSwInput;
            // 1st operate funciotn();

            while (pre_dipSwInput == dipSwInput){
                // 1.5 
                getDipSw(dipSwInput);
            }


            int successRate = 90;
            vector<int> trainings {1,2,3,4,1,2,3};
            vector<int> state = trainingResult(successRate, trainings); // 2nd operate function();
            for (int i=0; i<4; i++){
                creature.state[i] += state[i];
            }

        }

        // if creature.state > threshold 
        // and next status

    }

    // 3rd operate function();

    printClcd("Press any key to terminate Program");
    while (true){
        int tactSwInput = 0;
        getTactSw(tactSwInput);
        if (tactSwInput == 0) continue;
        else{
            cout << "Current tact switch input : " << tactSwInput << endl;
            break;
        }
    }
    clearClcd();
    
    vector<unsigned char> temp = {0x00, 0x00, 0x42, 0xA5, 0x00, 0x00, 0x00, 0x00};
    drawDotMTX(temp, 1000000);

    return 0;
}


/* Operate functions*/
vector<int> trainingResult(int successRate, vector<int> trainings){
    srand((unsigned int)time(NULL));
    int dice = random() % 100;
    vector<int> table(4, 0);

    // fail
    if ( successRate <= dice ){
        printClcd("Training failed");
        drawDotMTX(*TT, 1500000);
        cout << "Training failed: " << dice << endl;
        return table;
    }

    // 성공
    int volume = 10;
    int pre_training = 0;
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
            // cout << "current volume : " << volume << endl;
        }
        else {
            table[training] += volume;
            pre_training = training;
        }
    }
    printClcd("Training Success!");
    drawDotMTX(*smile, 1500000);
    // cout << "running : " << table[1] << endl;
    // cout << "flying : " << table[2] << endl;
    // cout << "swimming : " << table[3] << endl;
    return table;
}

/* IO functions */
int getDipSw(unsigned char& input){
    dipSw = open(dip, O_RDWR);
    if (dipSw < 0) {
        cout << "can't find Dev driver" << endl;
        return -1;
    }
    read(dipSw, &input, sizeof(input));
    close(dipSw);
    return 0;
}

int clearClcd(){
    clcds = open(clcd, O_RDWR);
    unsigned char clear[32];
    memset(clear, 0, 32);
    if (clcds < 0){
        cout << "can't find Dev dirver" << endl;
        return -1; 
    }

    write(clcds, &clear, sizeof(clear));
    close(clcds);
    return 0;
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