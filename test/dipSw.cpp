#include <iostream>
#include <vector>
#include <string>

#include <string.h>
#include <unistd.h> 			// POSIX 운영체제 API에 대한 액세스 제공
#include <fcntl.h> 			// 타겟시스템 입출력 장치 관련
#include <sys/types.h> 		// 시스템에서 사용하는 자료형 정보
#include <sys/ioctl.h> 		// 하드웨어의 제어와 상태 정보
#include <sys/stat.h> 		// 파일의 상태에 대한 정보
#include <stdlib.h>

#define dip "/dev/dipsw"	// Dip Switch

int dipSw;

int getDipWd(unsigned char& input){
    dipSw = open(dip, O_RDWR);
    while(1){
        read(dipSw, &input, sizeof(input));
        if (input) break;
    }
    close(dipSw);
    return 0;
}

int main() {
    unsigned char aa=0;
    getDipWd(aa);
    std::cout << (int)aa << std::endl;
    return 0;
}