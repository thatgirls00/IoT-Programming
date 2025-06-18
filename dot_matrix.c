// [3] 남은 시간 Dot-Matrix 표시
void write_dot_number(int num) {
    if (num < 0 || num > 9) return;
    int fd = open(DOT_DEVICE, O_WRONLY);
    if (fd < 0) return;
    write(fd, dot_patterns[num], 8);  // 8x8 숫자 표시
    close(fd);
}

// [3] Dot-Matrix 카운트다운 호출 위치 (게임 중)
int elapsed = time(NULL) - start_time;
int remaining = timeout - elapsed;
if (remaining >= 0 && remaining <= 9)
    write_dot_number(remaining);
