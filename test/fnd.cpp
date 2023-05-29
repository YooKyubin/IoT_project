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

using namespace std;

// 1        2       3       4       5       6       7       8       9       0       .
// 0xF9,    0xA4,   0xB0,   0x99,   ~0x6D,  ~0x7D,  ~0x07,  ~0x7F,  0x10,   0xC0,   0x80
vector<unsigned char> fnd_number {~0x3f, ~0x06, ~0x5b, ~0x4f, ~0x66, ~0x6d, ~0x7d, ~0x07, ~0x7f, ~0x67, ~0x00};
                                // {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x98};

#define fnd "/dev/fnd" 		// 7-Segment FND 

int fnds;

int main() {
    printFnd(1234, 1000000);
    printFnd(2345, 1000000);
    printFnd(3456, 1000000);
    printFnd(7890, 1000000);
    printFnd(0, 1000000);
    printFnd(90, 1000000);
    return 0;
}

int printFnd(int input, unsigned int sleepSec){
    vector<unsigned char> data(4, fnd_number[0]);
    data[0] = input / 1000  % 10;
    data[1] = input / 100   % 10;
    data[2] = input / 10    % 10;
    data[3] = input / 1     % 10;
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



