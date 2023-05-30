#include <iostream>
#include <vector>
#include <string>

#include <string.h>
#include <unistd.h> 			// POSIX 운영체제 API에 대한 액세스 제공
#include <fcntl.h> 			// 타겟시스템 입출력 장치 관련
#include <sys/types.h> 		// 시스템에서 사용하는 자료형 정보
#include <sys/ioctl.h> 		// 하드웨어의 제어와 상태 정보
#include <sys/stat.h> 		// 파일의 상태에 대한 정보
#include <time.h> 			// 시간 관련
#include <stdlib.h>

#define clcd "/dev/clcd" 	// Character LCD

using namespace std;

int fnds;
int clcds;
int dipSw;
int tactSw;
int dotMtx;

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