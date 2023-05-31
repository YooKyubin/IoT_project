#include <iostream>
#include <fcntl.h>			// 타겟시스템 입출력 장치 관련
#include <sys/types.h>		// 시스템에서 사용하는 자료형 정보
#include <sys/stat.h> 		// 파일의 상태에 대한 정보
#include <time.h> 			// 시간 관련
#include <unistd.h>         ///
#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define tact "/dev/tactsw" 	// Tact Switch
#define clcd "/dev/clcd" 	// Character LCD

using namespace std;

bool game_care();
int printClcd(string str);
int getTactSw(int& input);


int clcds;
int tactSw;
string train_str = "1.Run 2.Fly 3.Swim 4.Hunt*";

int main() {

	game_care();
	printClcd(train_str);

}

bool game_care() {

	string care_arr[4] = { "1.Feed", "2.Wash", "3.Play", "4.Sleep" };
	int random_index = rand() % care_arr->size();
	string care_str = care_arr[random_index];
	
	printClcd(care_str);

	int tactInput = 0; //tactswinput

	while (tactInput == 0) {
		getTactSw(tactInput);
		//cout << tactInput << endl;		
		// if (tactInput != 0) break;
	}

	if (random_index + 1 == tactInput) {
		cout << "care success" << endl;
		return true;
	}
	else {
		cout << "failure to care" << endl;
		return false;
	}

}

int printClcd(string str) {
	clcds = open(clcd, O_RDWR);

	if (clcds < 0) {
		cout << "can't find Dev dirver" << endl;
		return -1;
	}

	if (write(clcds, str.c_str(), 32) == -1) {
		cout << "file write error" << endl; // str.size()이 걸로 되는지 모르겠음
		return -1;
	}
	close(clcds);
	return 0;
}

int getTactSw(int& input) {
	tactSw = open(tact, O_RDWR);
	if (tactSw < 0) {
		cout << "can't find Dev driver" << endl;
		return -1;
	}
	read(tactSw, &input, sizeof(input));
	close(tactSw);
	return 0;
}
