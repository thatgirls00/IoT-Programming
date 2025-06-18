// [1-1] 단일 메시지를 Text LCD에 출력 (32자 고정 포맷)
void lcd_write_message(const char* msg) {
    int fd = open(LCD_DEVICE, O_WRONLY);
    if (fd < 0) return;
    char line[33] = {0};
    snprintf(line, sizeof(line), "%-32s", msg); // 공백 포함 32자 채우기
    write(fd, line, 32);
    close(fd);
}

// [1-2] 현재 턴과 스트라이크/볼/아웃 결과를 LCD에 출력 (상하 16자)
void write_turn_and_result_to_lcd(const char* turn, int strike, int ball, int out) {
    int fd = open(LCD_DEVICE, O_WRONLY);
    if (fd < 0) return;

    char upper[17] = {0}; // 상단 줄
    char lower[17] = {0}; // 하단 줄
    char full[33] = {0};  // 전체 출력 버퍼

    snprintf(upper, sizeof(upper), "%-16s", turn);                     // 예: "User Turn       "
    snprintf(lower, sizeof(lower), "S:%d B:%d O:%d   ", strike, ball, out); // 예: "S:1 B:2 O:0   "
    snprintf(full, sizeof(full), "%-16s%-16s", upper, lower);          // 32자 결합

    write(fd, full, 32);
    close(fd);
}

// [1-3] 게임 종료 요약 정보를 LCD에 출력 (정답 포함)
void write_game_summary_to_lcd(int tries, int* answer, int len, const char* result, time_t elapsed_sec) {
    int fd = open(LCD_DEVICE, O_WRONLY);
    if (fd < 0) return;
    char line[33] = {0};
    char upper[17] = {0}, lower[17] = {0};

    snprintf(upper, sizeof(upper), "%s T:%d %lds", result, tries, elapsed_sec); // 예: "Win T:5 20s"
    snprintf(lower, sizeof(lower), "Ans:"); // 정답 표시 앞부분

    for (int i = 0; i < len; i++) {
        char num[4];
        snprintf(num, sizeof(num), "%d ", answer[i]);
        strncat(lower, num, sizeof(lower) - strlen(lower) - 1);
    }

    snprintf(line, sizeof(line), "%-16s%-16s", upper, lower); // 32자 맞춰서 출력
    write(fd, line, 32);
    close(fd);
}
