// [3-1] 부저 동작 함수 선언부

// 지정된 시간(ms) 동안 부저를 울림
void buzz(int duration_ms);

// 정답 맞췄을 때 효과음 (짧은 부저음 3회 반복)
void buzz_success();

// 게임 실패 시 효과음 (긴 부저음 1회)
void buzz_fail();

// 시간 초과 시 효과음 (중간 길이 부저음 2회 반복)
void buzz_timeout();

// 숫자 입력 시 짧은 피드백 부저음
void buzz_input();
