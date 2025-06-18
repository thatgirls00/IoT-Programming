#include <stdio.h>

// [정답 디버깅 메시지 출력]
// 테스트용 정답을 터미널에 출력
// 실사용 시에는 주석 처리하거나 조건부로 출력
void print_debug_answer(int* answer, int len) {
    printf("정답 (Debug Only): ");
    for (int i = 0; i < len; i++) {
        printf("%d ", answer[i]);
    }
    printf("\n");
}
