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
#include <sys/time.h> 			// 시간 관련
#include <stdlib.h>

#define fnd "/dev/fnd" 		// 7-Segment FND 
#define dot "/dev/dot" 		// Dot Matrix
#define tact "/dev/tactsw" 	// Tact Switch
#define led "/dev/led" 		// LED 
#define dip "/dev/dipsw"	// Dip Switch
#define clcd "/dev/clcd" 	// Character LCD

using namespace std;

void gameDescription();
bool game_care();
void trainingResult(int successRate, vector<int> trainings);
bool train(int& successRate, vector<int>& trainings, string& trainClcd, unsigned char pre_dipSwInput, unsigned char& dipSwInput);
bool check_gameover_1();
int determineNext();
void evolAnimation(vector<unsigned char> cur, vector<unsigned char> next);
void IdleAnimation(
    vector<unsigned char> pic, vector<unsigned char> shiftedPic,
    vector<unsigned char>& draw, bool& toggle);

void print_dot_mtx_gameover();
int printFnd(int input, unsigned int sleepSec);
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

unsigned char smile[8] = {0x00, 0x00, 0x42, 0xA5, 0x00, 0x00, 0x00, 0x00};
unsigned char TT[8] = {0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x00, 0x00};
// unsigned char fnd_number[] = {~0x3f, ~0x06, ~0x5b, ~0x4f, ~0x66, ~0x6d, ~0x7d, ~0x07, ~0x7f, ~0x67, ~0x00};
unsigned char fnd_number[10] = { 
    (unsigned char)~0x3f, 
    (unsigned char)~0x06, 
    (unsigned char)~0x5b, 
    (unsigned char)~0x4f, 
    (unsigned char)~0x66,
    (unsigned char)~0x6d, 
    (unsigned char)~0x7d, 
    (unsigned char)~0x07, 
    (unsigned char)~0x7f, 
    (unsigned char)~0x67 
};
vector<vector<unsigned char>> figure { 
    {0x00, 0x3c, 0x7e, 0x5a, 0x66, 0x7e, 0x66, 0x42},   //유년기
    {0x00, 0x84, 0x58, 0x20, 0x6E, 0x1F, 0x29, 0x2A},   //사슴, run, fly
    {0x00, 0x00, 0x8E, 0x5F, 0x00, 0x29, 0x00, 0x00},   //거북이, run, swim
    {0x0E, 0x11, 0x20, 0x36, 0x70, 0x78, 0x68, 0x08},   //독수리,   fly, run
    {0x00, 0x00, 0x67, 0x18, 0x24, 0x02, 0x00, 0x00},   //잠자리, fly  swim
    {0x00, 0x00, 0x08, 0x3D, 0x6E, 0x11, 0x00, 0x00},   //돌고래, swim, run
    {0x1C, 0x22, 0x41, 0x7F, 0x2A, 0x2A, 0x2A, 0x2A},   //해파리, swim, fly
    {0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81},   //게임오버 표시 X
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    //초기화
};
vector<vector<unsigned char>> faces {
    {0x18, 0x34, 0x7a, 0x7a, 0xff, 0xff, 0x7e, 0x3c},   // egg2
    {0x00, 0x66, 0x3c, 0x7e, 0x5a, 0x7e, 0x66, 0x7e}    // 새 파일 유년기5
};
vector<vector<unsigned char>> shiftedFaces{
    {0x70, 0xcc, 0xf6, 0xff, 0xff, 0x7f, 0x7f, 0x3e},   // 새 파일 2
    {0x00, 0x66, 0x3c, 0x5a, 0x7e, 0x66, 0x66, 0x7e}    //새파일6 
};

struct Creature{
    int status;
    string name;
    // unsigned char face[8];
    vector<unsigned char> face;
    vector<unsigned char> shiftedFace;
    vector<int> state;

    void init(int current){
        status = current;
        state.assign(4,0);
        face = faces[status];
        shiftedFace = shiftedFaces[status];
    }
};

Creature creature;
struct timeval cur, start;

int main() {
    
    gameDescription();
    clearClcd();

    // GAME START
    bool gameOver = false;
    for (int status=0; status < 2; status++){
        creature.init(status);

        // 애니메이션을 위한 변수
        vector<unsigned char> draw = creature.face;
        // vector<unsigned char> shiftedPic(8);
        bool toggle = true;
        // for (int i=0; i<8; i++) shiftedPic[i] = creature.face[i] << 1;

        int day = 0;
        unsigned char dipSwInput;
        getDipSw(dipSwInput);
        if (dipSwInput != 0){
            printClcd("  Please init      dip switch   ");
            gettimeofday(&start, NULL);
            while(dipSwInput != 0){
                // drawDotMTX(creature.face, 250000); // dot 매트릭스에 계속 띄워두기 위함
                IdleAnimation(creature.face, creature.shiftedFace, draw, toggle);
                getDipSw(dipSwInput);
            }
            clearClcd();
        }

        while (dipSwInput < 255){ // repeat 8
            unsigned char pre_dipSwInput = dipSwInput;
            int successRate = 90; // 성공확률이 110이면 이상하다는 의견을 반영하여 90 시작
            if (game_care()) successRate += 10;

            vector<int> trainings;
            string trainClcd = "Today's training : ";
            while (train(successRate, trainings, trainClcd, pre_dipSwInput, dipSwInput)) {
                // dipSw가 변경될 때 가지 반복(대기)
            }

            trainingResult(successRate, trainings); // 2nd operate function();
        }


        gameOver = check_gameover_1();
        if (gameOver) break;
        else if(creature.status == 0){
            evolAnimation(faces[0], faces[1]);
        }
    }

    if (!gameOver){
        // 3rd operate function();
        int next = determineNext();
        //int next = 4;
        cout << next << endl;
        if (next != 0){
            vector<string> animal {
                "      deer      ", 
                "     turtle     ", 
                "     eagle      ", 
                "    drgonfly    ",
                "     dolphin    ", 
                "    jellyfish   "};
            evolAnimation(creature.face, figure[next]);
            printClcd(" Your pet bcame " + animal[next-1]);
            int tatcSwInput = 0;

            bool toggle = true;
            vector<unsigned char> draw = figure[next];
            vector<unsigned char> shiftedPic(8);
            for (int i=0; i<8; i++) shiftedPic[i] = figure[next][i] << 1;
            gettimeofday(&start, NULL);
            while (tatcSwInput == 0){
                IdleAnimation(figure[next], shiftedPic, draw, toggle);
                getTactSw(tatcSwInput);
            }
        }
    }
    

    // GAME END
    printClcd("Press any key to terminate");
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
 

    return 0;
}


/* Operate functions*/
void IdleAnimation(
    vector<unsigned char> pic, vector<unsigned char> shiftedPic,
    vector<unsigned char>& draw, bool& toggle){

    gettimeofday(&cur, NULL);
    if (toggle){
        if (cur.tv_sec - start.tv_sec >= 3 && cur.tv_usec > start.tv_usec){
            toggle = !toggle;
            gettimeofday(&start, NULL);
            draw = shiftedPic;
        }
    }
    else {
        if (cur.tv_sec - start.tv_sec >= 1 && cur.tv_usec > start.tv_usec){
            toggle = !toggle;
            gettimeofday(&start, NULL);
            draw = pic;
        }
    }
    drawDotMTX(draw, 250000);

}

void gameDescription(){
    printClcd("    type of          cares      ");
    int tactSwInput = 0;
    while (tactSwInput == 0){
        getTactSw(tactSwInput);
    }

    tactSwInput = 0;
    printClcd("1. feed 2. wash 3. play 4. sleep");
    while (tactSwInput == 0){
        getTactSw(tactSwInput);
    }

    tactSwInput = 0;
    printClcd("    type of        trainings    ");
    while (tactSwInput == 0){
        getTactSw(tactSwInput);
    }

    tactSwInput = 0;
    printClcd("1. run   2. fly 3. swim  4. hunt");
    while (tactSwInput == 0){
        getTactSw(tactSwInput);
    }
}

bool check_gameover_1()
{
    int threshold_score_run = 3;  //달리기 점수 임계값
    int threshold_score_fly = 3;  //비행 점수 임계값
    int threshold_score_swim = 3; //수영 점수 임계값
    int score_run = creature.state[1];
    int score_fly = creature.state[2];
    int score_swim = creature.state[3];

    if( (score_run < threshold_score_run) ||(score_fly<threshold_score_fly) || (score_swim < threshold_score_swim) )
    {
        //game_over
        print_dot_mtx_gameover();
        return true;
    }
    else
    {
        return false;
    }
}

int determineNext(){
    int score_run = creature.state[1];
    int score_fly = creature.state[2];
    int score_swim = creature.state[3];
    if(score_run == score_fly || score_fly == score_swim || score_swim == score_run)
    { 
        //동점인 경우 순위가 정해지지 않기 때문에 게임 오버
        print_dot_mtx_gameover();
        return 0;
    }

    if( score_run > score_fly && score_run > score_swim)
    {   // score_run 이 1순위 일 때
        if(score_fly > score_swim)
        {   //score_fly 가 2 순위 일 때     >>      사슴
            return 1;
        }
        else if(score_fly < score_swim)
        {   //score_swim 이 2순위 일 때     >>      거북이
            return 2;
        }
    }
    else if( score_fly > score_run && score_fly > score_swim)
    {
        // score_fly 가 1순위 일 때
        if(score_run > score_swim)
        {   //score_run 가 2 순위 일 때     >>      독수리
            return 3;
        }
        else if(score_run < score_swim)
        {   //score_swim 이 2순위 일 때     >>      잠자리
            return 4;
        }
    }
    else if( score_swim > score_run && score_swim > score_fly)
    {
        // score_swim 가 1순위 일 때
        if(score_run > score_fly)
        {   //score_run 가 2 순위 일 때     >>      돌고래
            return 5;
        }
        else if(score_run < score_fly)
        {   //score_fly 이 2순위 일 때      >>      해파리
            return 6;
        }
    }
}

void print_dot_mtx_gameover() // 게임 오버 표시 도트 매트릭스 표현  X 표시 점멸 2번 후 초기화
{
    printClcd("   Game Over    ");

    dotMtx = open(dot, O_RDWR);
    write(dotMtx, figure[7].data(), 8); 
    usleep(500000); 
    write(dotMtx, figure[8].data(), 8); 
    usleep(500000); 
    write(dotMtx, figure[7].data(), 8); 
    usleep(500000); 
    write(dotMtx, figure[8].data(), 8); 
    usleep(500000);
    close(dotMtx);
    return;
}

bool game_care() {
    srand((unsigned int)time(NULL));
	string care_arr[4] = {"  My pet looks      hungry", 
                            "  My pet looks      dirty", 
                            "  My pet looks      bored", 
                            "  My pet looks      sleepy"};// { "1.Feed", "2.Wash", "3.Play", "4.Sleep" };
	int random_index = rand() % 4;
	string care_str = care_arr[random_index];
	
	printClcd(care_str);

	int tactInput = 0;

    vector<unsigned char> draw = creature.face;
    // vector<unsigned char> shiftedPic(8);
    bool toggle = true;
    // for (int i=0; i<8; i++) shiftedPic[i] = creature.face[i] << 1;
    gettimeofday(&start, NULL);
	while (tactInput == 0) {
		getTactSw(tactInput);
		usleep(1000);
        // drawDotMTX(creature.face, 250000);
        IdleAnimation(creature.face, creature.shiftedFace, draw, toggle);
        if ( tactInput > 4 ){
            //잘못된 입력
            printClcd("  Wrong Input! ");
            drawDotMTX(figure[7], 500000);
            // usleep(500000);
            tactInput = 0;
            printClcd(care_str);
        }
	}

	if (random_index + 1 == tactInput) {
		printClcd("Success to care!");
        drawDotMTX(*smile, 1500000);
		return true;
	}
	else {
		printClcd(" Failed to care ");
        drawDotMTX(*TT, 1500000);
		return false;
	}

}

// 현재 예약된 훈련 목록을 clcd로 보여줌, dotmtx로 현재 얼굴 보여줌, FND로 현재 성공확률 보여줌
bool train(int& successRate, vector<int>& trainings, 
    string& trainClcd, unsigned char pre_dipSwInput, unsigned char& dipSwInput){

    // if ( 돌봐주기 성공 ) { successRate += 10; }
    vector<unsigned char> draw = creature.face;
    // vector<unsigned char> shiftedPic(8);
    bool toggle = true;
    // for (int i=0; i<8; i++) shiftedPic[i] = creature.face[i] << 1;

    int training = 0;
    printClcd(trainClcd);
    printFnd(successRate, 1000000);
    gettimeofday(&start, NULL);
    while (training == 0){
        getTactSw(training);
        usleep(1000);
        // drawDotMTX(creature.face, 250000);
        IdleAnimation(creature.face, creature.shiftedFace, draw, toggle);

        getDipSw(dipSwInput);
        if (pre_dipSwInput != dipSwInput){
            if (pre_dipSwInput < dipSwInput) { return false; }
            else {
                printClcd("   roll back       dip switch   ");
                while( pre_dipSwInput != dipSwInput ){
                    getDipSw(dipSwInput);
                    drawDotMTX(figure[7], 250000);
                }
                printClcd(trainClcd);
            }
        }
    }

    if ( training > 4 ){
        //잘못된 입력
        printClcd("  Wrong Input! ");
        drawDotMTX(figure[7], 500000);
        // usleep(500000);
    }
    else{
        trainings.push_back(training);
        // trainClcd += to_string(training);
        trainClcd += to_string(training) + " ";
        successRate -= 10;
        if (training == 4){
            successRate += 3; // 사냥은 7 감소
        }
    } 
    return true;
}

void trainingResult(int successRate, vector<int> trainings){
    srand((unsigned int)time(NULL));
    int dice = random() % 100;
    vector<int> table(4, 0);

    cout << "successRate: " << dice << endl;
    // fail
    if ( successRate <= dice ){
        printClcd("Training failed");
        drawDotMTX(*TT, 1500000);
        return;
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
    
    for (int i=1; i<4; i++){
        creature.state[i] += table[i];
    }

    printClcd("Training Success!");
    drawDotMTX(*smile, 700000);
    printClcd("   running :        " + to_string(creature.state[1]) + " (+" + to_string(table[1]) + ")" ); // running : 32 ( +3 )
    drawDotMTX(*smile, 700000);
    printClcd("   flying :         " + to_string(creature.state[2]) + " (+" + to_string(table[2]) + ")" ); 
    drawDotMTX(*smile, 700000);
    printClcd("   swimming :       " + to_string(creature.state[3]) + " (+" + to_string(table[3]) + ")" ); 
    drawDotMTX(*smile, 700000);
    clearClcd();
    // cout << "running : " << table[1] << endl;
    // cout << "flying : " << table[2] << endl;
    // cout << "swimming : " << table[3] << endl;
    return;
}

void evolAnimation(vector<unsigned char> cur, vector<unsigned char> next){
    vector<unsigned char> pic = cur;
    for (int i=7; i >= 0; i--){
        pic[i] = (unsigned char)255;
        drawDotMTX(pic, 250000);
    }
    for (int i=0; i < 8; i++){
        pic[i] = next[i];
        drawDotMTX(pic, 250000);
    }
}

/* IO functions */
int printFnd(int input, unsigned int sleepSec) {
    vector<unsigned char> data(4, fnd_number[0]);

    int thousands = input / 1000 % 10;
    int hundreds = input / 100 % 10;
    int tens = input / 10 % 10;
    int ones = input / 1 % 10;
    
    if (thousands != 0) {
        data[0] = fnd_number[thousands];
        data[1] = fnd_number[hundreds];
        data[2] = fnd_number[tens];
        data[3] = fnd_number[ones];
    } else if (thousands == 0 && hundreds != 0){
        data[0] = ~((unsigned char)0);
        data[1] = fnd_number[hundreds];
        data[2] = fnd_number[tens];
        data[3] = fnd_number[ones];
    } else if (thousands == 0 && hundreds == 0 && tens != 0) {
        data[0] = ~((unsigned char)0);
        data[1] = ~((unsigned char)0);
        data[2] = fnd_number[tens];
        data[3] = fnd_number[ones];
    } else if (thousands == 0 && hundreds == 0 && tens == 0){
        data[0] = ~((unsigned char)0);
        data[1] = ~((unsigned char)0);
        data[2] = ~((unsigned char)0);
        data[3] = fnd_number[ones];
   
    }  /* else if (thousands == 0 && hundreds == 0 && tens == 0 && ones == 0){
        data[0] = ~((unsigned char)0);
        data[1] = ~((unsigned char)0);
        data[2] = ~((unsigned char)0);
        data[3] = fnd_number[ones];
    }
    */

    fnds = open(fnd, O_RDWR);
    if (fnds < 0) {
    	cout << "can't find Dev driver" << endl;
    	return -1;
    }
    write(fnds, data.data(), 4);
    usleep(sleepSec);
    close(fnds);
    return 0;
}

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
        cout << "can't find Dev driver" << endl;
        return -1; 
    }

    write(clcds, &clear, sizeof(clear));
    close(clcds);
    return 0;
}

int printClcd(string str){
    clcds = open(clcd, O_RDWR);

    if (clcds < 0){
        cout << "can't find Dev driver" << endl;
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
        cout << "can't find Dev driver" << endl;
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
        cout << "can't find Dev driver" << endl;
        return -1; 
    }
    write(dotMtx, input.data(), 8);
    usleep(sleepSec);
    close(dotMtx);
    return 0;
}
