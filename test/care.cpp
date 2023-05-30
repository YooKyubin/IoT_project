#include <iostream>
#include <fcntl.h>			// 타겟시스템 입출력 장치 관련
#include <sys/types.h>		// 시스템에서 사용하는 자료형 정보
#include <sys/stat.h> 		// 파일의 상태에 대한 정보
#include <time.h> 			// 시간 관련
//#include <random>
#include <unistd.h>
#include <vector>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define tact "/dev/tactsw" 	// Tact Switch
#define clcd "/dev/clcd" 	// Character LCD

using namespace std;

int clcd_1;
int tactsw;
int proba = 100;


// 함수 선언부
//int clcd_care(); // 돌보기 항목 CLCD에 띄우기
int game_care();
int clcd_train();

int main() {

	game_care();
	clcd_train();

}


int game_care() {

	string care_arr[4] = { "1.Feed", "2.Wash", "3.Play", "4.Sleep" };
	int random_index = rand() % care_arr->size();
	string care_str = care_arr[random_index];
	const char* a = care_str.c_str();

	clcd_1 = open(clcd, O_RDWR);

	if (clcd_1 < 0) // 예외처리
	{
		cout << "can't find device driver.\n" << endl;
		return -1;
	}
	write(clcd_1, a, strlen(a));
	close(clcd_1);

	unsigned char c;

	if ((tactsw = open(tact, O_RDWR)) < 0) {
		cout << "can't find device driver.\n" << endl;
	}
	while (1) {
		read(tactsw, &c, sizeof(c));
		if (c) break;
	}
	switch (c)
	{
	case 1:
		if (care_str == "1.Feed") {
			proba += 10;
			cout << "care success" << endl;
			return true;
		}
		else {
			cout << "failure to care" << endl;
			return false;
		}
	case 2:
		if (care_str == "2.Wash") {
			proba += 10;
			cout << "care success" << endl;
			return true;
		}
		else {
			cout << "failure to care" << endl;
			return false;
		}
	case 3:
		if (care_str == "3.Play") {
			proba += 10;
			cout << "care success" << endl;
			return true;
		}
		else {
			cout << "failure to care" << endl;
			return false;
		}
	case 4:
		if (care_str == "4.Sleep") {
			proba += 10;
			cout << "care success" << endl;
			return true;
		}
		else {
			cout << "failure to care" << endl;
			return false;
		}
	}

}

/*int clcd_care() {
	clcd_1 = open(clcd, O_RDWR);

	if (clcd_1 < 0) // 예외처리
	{
		cout << "can't find device driver.\n" << endl;
		return -1;
	}

	write(clcd_1, random_string, strlen(random_string));
	close(clcd_1);
	return 0;
} */

int clcd_train() {
	char train_text[30] = "1.Run 2.Fly 3.Swim 4.Hunt*";

	clcd_1 = open(clcd, O_RDWR);

	if (clcd_1 < 0) // 예외처리
	{
		cout << "can't find device driver.\n" << endl;
		return -1;
	}

	write(clcd_1, train_text, strlen(train_text));
	close(clcd_1);
	return 0;
}
