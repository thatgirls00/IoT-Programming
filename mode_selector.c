// [2-1] 난이도 선택 처리 함수
// 사용자가 스위치를 통해 1~4번 중 난이도를 선택하면 해당 모드를 반환
int select_mode() {
    // LCD에 난이도 안내 메시지 출력
    lcd_write_message("1.Easy 2.Normal 3.Hard 4.AI");

    // 터미널에도 안내 출력
    printf("난이도를 선택하세요: 1~4번\n");

    int mode = 0;
    while (1) {
        // 스위치 입력을 받아 mode에 저장
        mode = read_single_switch();

        // 유효한 입력(1~4)일 경우
        if (mode >= 1 && mode <= 4) {
            buzz_input();   // 짧은 부저음으로 입력 피드백 제공
            return mode;    // 선택된 난이도 반환
        }

        // 너무 빠른 반복 방지 (0.2초 대기)
        usleep(200000);
    }
}
