/*
유년기 >> 성체 진화할 때 테스트 코드
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include <string.h>
#include <time.h>

#define dot "/dev/dot"
#define clcd "/dev/clcd"
#define tact "/dev/tactsw"
#define fnd "/dev/fnd"

int dot_mtx = 0;

unsigned char c[9][8] = { 
    {0x00, 0x3c, 0x7e, 0x5a, 0x66, 0x7e, 0x66, 0x42},   //유년기
    {0x00, 0x84, 0x58, 0x20, 0x6E, 0x1F, 0x29, 0x2A},   //사슴, run, fly
    {0x00, 0x00, 0x8E, 0x5F, 0x00, 0x29, 0x00, 0x00},   //거북이, run, swim
    {0x0E, 0x11, 0x20, 0x36, 0x70, 0x78, 0x68, 0x08},   //독수리,   fly, run
    {0x00, 0x00, 0x67, 0x18, 0x24, 0x02, 0x00, 0x00},   //잠자리, fly  swim
    {0x00, 0x00, 0x08, 0x3D, 0x6E, 0x11, 0x00, 0x00},   //돌고래, swim, run
    {0x1C, 0x22, 0x41, 0x7F, 0x2A, 0x2A, 0x2A, 0x2A},   //해파리, swim, fly
    {0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81},   // 게임오버 표시 X
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}    //초기화
};

void print_dot_mtx_gameover()
{
   write(dot_mtx, &c[8], sizeof(c[8])); 
    sleep(1); 
    write(dot_mtx, &c[9], sizeof(c[8])); 
    sleep(1); 
    write(dot_mtx, &c[8], sizeof(c[8])); 
    sleep(3); 
    write(dot_mtx, &c[9], sizeof(c[8])); 
    usleep(500000); 
    close(dot_mtx); 
    return;
}


//알 >> 유년기 게임오버 판별
int check_gameover_1(unsigned int score_run,unsigned int score_fly,unsigned int score_swim)
{
    unsigned int threshold_score_run = 3;  //달리기 점수 임계값
    unsigned int threshold_score_fly = 3;  //비행 점수 임계값
    unsigned int threshold_score_swim = 3; //수영 점수 임계값

    if( (score_run < threshold_score_run) ||(score_fly<threshold_score_fly) || (score_swim < threshold_score_swim) )
    {
        //game_over
        dot_mtx = open(dot, O_RDWR);
  		if (dot_mtx < 0) 
        {
            printf("Can't open dot matrix.\n"); 
            exit(0);
        }
		
        // 게임 오버 표시 도트 매트릭스 표현  X 표시 점멸 2번 후 초기화
        print_dot_mtx_gameover();


        return 0;
    }
    else
    {
        // 알에서 부화함
        // 도트 매트릭스에 유년기 캐릭터 표시
        dot_mtx = open(dot, O_RDWR);
  		if (dot_mtx < 0) 
        {
            printf("Can't open dot matrix.\n"); 
            exit(0);
        }
		
        write(dot_mtx, &c[0], sizeof(c[0])); 
        usleep(500000); 
        
  		close(dot_mtx);
        return 1;
    }
}



//유년기 >> 최종진화 게임오버 판별
int check_gameover_2(unsigned int score_run,unsigned int score_fly,unsigned int score_swim)
{
    unsigned int threshold_score_run = 5;  //달리기 점수 임계값
    unsigned int threshold_score_fly = 5;  //비행 점수 임계값
    unsigned int threshold_score_swim = 5; //수영 점수 임계값

    if( (score_run < threshold_score_run) ||(score_fly<threshold_score_fly) || (score_swim < threshold_score_swim) )
    {
        //game_over

        // 게임 오버 표시 도트 매트릭스 표현  X 표시 점멸 2번 후 초기화
        print_dot_mtx_gameover();

        return 0;
    }
    else
    {
        //경우의 수 대로 진화된 상태 보여줌
        
        //1 순위가 동점인 경우
        //2 순위가 동점인 경우 있을 수 있음
        //셋다 점수가 같을 수도 있음
        //동점인 경우 게임오버

        if(score_run == score_fly || score_fly == score_swim || score_swim == score_run)
        {
            //동점인 경우 순위가 정해지지 않기 때문에 게임 오버

            // 게임 오버 표시 도트 매트릭스 표현  X 표시 점멸 2번 후 초기화
            print_dot_mtx_gameover();

            return 0;
        }

        if( score_run > score_fly && score_run > score_swim)
        {   // score_run 이 1순위 일 때
            if(score_fly > score_swim)
            {   //score_fly 가 2 순위 일 때     >>      사슴

                write(dot_mtx, &c[1], sizeof(c[1])); 
                usleep(500000); 
                close(dot_mtx);
                sleep(2);
                
                return 1;
            }
            else if(score_fly < score_swim)
            {   //score_swim 이 2순위 일 때     >>      거북이

                write(dot_mtx, &c[2], sizeof(c[2])); 
                usleep(500000); 
                close(dot_mtx);
                sleep(2);

                return 2;
            }
        }
        else if( score_fly > score_run && score_fly > score_swim)
        {
           // score_fly 가 1순위 일 때
            if(score_run > score_swim)
            {   //score_run 가 2 순위 일 때     >>      독수리

                write(dot_mtx, &c[3], sizeof(c[3])); 
                usleep(500000); 
                close(dot_mtx);
                sleep(2);

                return 3;
            }
            else if(score_run < score_swim)
            {   //score_swim 이 2순위 일 때     >>      잠자리

                write(dot_mtx, &c[4], sizeof(c[4])); 
                usleep(500000); 
                close(dot_mtx);
                sleep(2);
                return 4;
            }
        }
        else if( score_swim > score_run && score_swim > score_fly)
        {
           // score_swim 가 1순위 일 때
            if(score_run > score_fly)
            {   //score_run 가 2 순위 일 때     >>      돌고래

                write(dot_mtx, &c[5], sizeof(c[5])); 
                usleep(500000); 
                close(dot_mtx);
                sleep(2);

                return 5;
            }
            else if(score_run < score_fly)
            {   //score_fly 이 2순위 일 때      >>      해파리

                write(dot_mtx, &c[6], sizeof(c[6])); 
                usleep(500000); 
                close(dot_mtx);
                sleep(2);

                return 6;
            }
        }
    }
}


void main()
{
    int run = 6;    //test value
    int fly = 8;    //test value
    int swim = 10;   //test value

    //check_gameover_2(unsigned int score_run,unsigned int score_fly,unsigned int score_swim)
    if(check_gameover_2(run, fly, swim))   //  함수에 현재 점수를 run, fly, swim 순으로 넣어주면 게임오버를 판단함 (유년기 >> 최종 진화)
    {
       // 함수 값이 0이면 게임 오버
        printf("Evolution");
    }
    else
    {
        printf("Game Over");
        return ;
    }
    return ;
}
