#include <iostream>
#include <fcntl.h>
#include <unistd.h>

#define clcd "/dev/clcd"

int main() {
    int clcd_1;
    clcd_1 = open(clcd, O_RDWR); 

    if (clcd_1 < 0) {
        std::cout << "디바이스 드라이버가 없습니다.\n";
        return 0;
    }

    write(clcd_1, "Hello World", 12);

    close(clcd_1);
    return 0;
}