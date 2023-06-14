# 프로그램 설명

게임 시작 전 간단 메뉴얼 clcd로 출력

<img src="imgs/type_of_cares.jpg" width="300px"/> <img src="imgs/care_list.jpg" width="270px"/>

<img src="imgs/type_of_train.jpg" width="300px"/> <img src="imgs/train_list.jpg" width="300px"/>


dip switch 초기화 확인

<p>
	<img src="gif/init_dipSW.gif" width="500px">
</p>


돌봐주기 
- 질문

<p>
	<img src="gif/돌보기.gif" width="500px">
</p>

- 성공

<p>
	<img src="gif/돌보기성공-.gif" width="500px">
</p>


- 실패

- 예외처리
 
<p>
	<img src="gif/돌보기_예외처리.gif" width="500px">
</p>


훈련하기
- 성공확률
- tact switch 상호작용
- dip switch 작동시 성공(clcd, dot matrix)
- dip switch 작동시 실패(clcd, dot matrix)

<p>
	<img src="gif/훈련성공.gif" width="500px">
</p>

- 예외처리

<p>
	<img src="gif/훈련_예외처리.gif" width="500px">
</p>

dip switch 8개 모두 작동 시 진화
- 진화 애니메이션
  - 알 -> 유년기
<p>
	<img src="gif/evolution.gif" width="400px">
</p>
<br/>
<br/>
<br/>

  - 유년기 -> 성체
<p>
	<img src="gif/evolution2.gif" width="400px">
</p>


진화 실패모습 (게임 오버)

<p>
	<img src="gif/gameover.gif" width="400px">
</p>


# 펫 애니메이션
<br/>

### 알
<p>
	<img src="gif/image_egg.gif" width="300px">        <img src="gif/egg.gif" width="300px">
</p>

<br/>
<br/>
<br/>

### 유년기

<p>
	<img src="gif/image_baby.gif" width="300px">         <img src="gif/child.gif" width="300px">
</p>

<br/>
<br/>
<br/>

### 사슴

<p>
	<img src="gif/image_deer.gif" width="300px">         <img src="gif/doot_deer.gif" width="300px">
</p>

<br/>
<br/>
<br/>

### 거북이

<p>
	<img src="gif/image_turtle.gif" width="300px">         <img src="gif/dot_turtle.gif" width="300px">
</p>

<br/>
<br/>
<br/>

### 독수리

<p>
	<img src="gif/image_eagle.gif" width="300px">         <img src="gif/dot_eagle.gif" width="300px">
</p>

<br/>
<br/>
<br/>

### 잠자리

<p>
	<img src="gif/image_dragonfly.gif" width="300px">         <img src="gif/dot_dragonfly.gif" width="300px">
</p>

<br/>
<br/>
<br/>

### 해파리

<p>
	<img src="gif/image_jellyfish.gif" width="300px">         <img src="gif/dot_jellyfish.gif" width="300px">
</p>

<br/>
<br/>
<br/>

### 돌고래

<p>
	<img src="gif/image_dolphin.gif" width="300px">         <img src="gif/dot_dolphin.gif" width="300px">
</p>

<br/>
<br/>
<br/>


# 개발 과정
## 2주차

## 3주차

## 4주차
### 개발 내용
- 게임 시작 전 간단 게임 메뉴얼 출력
- 각종 동작 함수들을 합쳐 하나의 프로그램으로 작정
- 프로그램 실행 중 **항상** dot matrix에 애완동물 출력
- 애완동물의 간단한 애니메이션 추가

### 문제 발생, 목표
- 하나의 프로그램으로 작성하는 도중 dip switch 입력과 tact swtich 입력을 확인하기 위한 중첩 반복문에서 두 루프를 한번에 탈출하지 못하는 문제 발생
- 애니메이션의 움직임 주기를 `clock()` 함수를 사용하였는데 실행 중 `clock()`함수가 올바른 값을 리턴하지 못함

## 5주차
### 개발 내용
- 중첩 반복문 중 내부의 반복문을 함수로 작성하여 tact switch의 입력을 확인하고, dip switch 작동에 의해 2중 반복문을 탈출할 조건이 충족되면 `false` 를 리턴하여 중첩 반복문을 탈출
- 애니메이션 주기를 `clock()` 대신 `gettimeofday()` 함수를 이용하여 애니메이션 주기 통제
- 진화 애니메이션 추가
- fnd 출력 함수 재 작성

# 핵심 코드

애니메이션 구현
```c++
struct timeval cur, start;

int main() {
	gettimeofday(&start, NULL);
	while (tatcSwInput == 0){
		IdleAnimation(figure[next], shiftedPic, draw, toggle);
		getTactSw(tatcSwInput);
	}
	return 0;
}

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
```
>단순히 `sleep()`으로 애니메이션의 지속시간을 설정하지 않았기 때문에 
>tact switch, dip switch의 입력을 받으면서 dot matrix에서 애니메이션을 연출 할 수 있다.
>기존 `drawDotMTX()` 함수를 사용하는 곳이면 어디든 대체하여 사용이 가능하다.

중첩 반복문 탈출
```c++
int main() {
	while (train(successRate, trainings, trainClcd, pre_dipSwInput, dipSwInput)) {
		// dipSw가 변경될 때 까지 반복(대기)
	}
	return 0;
}

bool train(int& successRate, vector<int>& trainings,
	string& trainClcd, unsigned char pre_dipSwInput, unsigned char& dipSwInput){
	
	int training = 0;
	printClcd(trainClcd);
	printFnd(successRate, 1000000);
	gettimeofday(&start, NULL);
	while (training == 0){
		getTactSw(training);
		usleep(1000);
	
		getDipSw(dipSwInput);
		if (pre_dipSwInput != dipSwInput){
			return false;
		}
	}
	
	if ( training > 4 ){
		//잘못된 입력
		printClcd("  Wrong Input! ");
		usleep(500000);
	}
	else{
		trainings.push_back(training);
		trainClcd += to_string(training) + " ";
		successRate -= 10;
		if (training == 4){
			successRate += 3; // 사냥은 7 감소
		}
	}
	return true;
}
```
>tact switch 입력시 내부 반복문이 종료, dip switch 입력시 전체 반복문이 종료된다.

# 프로그램 실행 영상

# 완성도 및 독창성
## 완성도
- 게임 시작 전 간단한 설명 제시
- `srand((unsigned int)time(NULL))`를 이용하여 다양한 랜덤값 생성
- `1`, `2`, `3`, `4`를 제외한 다른 tact swtich 입력에 대한 예외 처리
- 생동감을 위한 애완동물의 IDLE 애니메이션 제작
- 애완 동물의 진화 연출 제작
- 돌봐주기, 훈련의 성공과 실패 시의 연출

## 독창성
- 프로젝트의 아이디어(주제) 중복 없음
- 기존에 없던 게임, 새로운 규칙 작성성
- 기본적인 입출력 장치의 사용방법을 제외하고 타 프로젝트의 코드를 참고하지 않음
- 우리 프로젝트만의 애니메이션 알고리즘 작성
- 알, 유년기 모습의 dot 이미지 직접 생성

# 아쉬운 점
roll back dip switch

# 참고 문헌

