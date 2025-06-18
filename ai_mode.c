// [4-4] AI용 난수 입력 생성 함수 선언 (선언부)
void ai_guess_number(int* guess, int len);

// [4-4] AI 모드용 숫자 생성 (함수 정의)
// 중복되지 않는 len 자리의 숫자를 랜덤으로 생성하여 guess 배열에 저장
void ai_guess_number(int* guess, int len) {
    int used[10] = {0}, count = 0;
    while (count < len) {
        int num = (rand() % 9) + 1;  // 1~9 사이 숫자 생성
        if (!used[num]) {
            guess[count++] = num;
            used[num] = 1;
        }
    }
}

// [4-4] 사용자 또는 AI 턴에 따라 입력 방식 분기 (main 게임 루프 내 사용 부분)
if (!ai_mode || (tries % 2 == 0)) {
    // 유저 턴
    write_turn_and_result_to_lcd("User Turn", 0, 0, 0);
    // 사용자 입력 처리 루프 (스위치 기반 입력 등)
    while (1) {
        int elapsed = time(NULL) - start_time;
        int remaining = timeout - elapsed;
        if (remaining >= 0 && remaining <= 9)
            write_dot_number(remaining);
        if (elapsed >= timeout) { timed_out = 1; break; }

        int num = read_single_switch();
        if (num != -1) {
            buzz_input();
            int duplicate = 0;
            for (int i = 0; i < input_count; i++)
                if (input[i] == num) duplicate = 1;

            if (!duplicate && input_count < len)
                input[input_count++] = num;

            write_to_fnd(input, len);
            if (input_count == len) break;
        }
        usleep(200000);
    }
} else {
    // AI 턴
    write_turn_and_result_to_lcd("AI Turn", 0, 0, 0);

    ai_guess_number(input, len);     // AI가 숫자 생성
    printf("[AI 입력]: ");
    for (int i = 0; i < len; i++) printf("%d ", input[i]);
    printf("\n");

    write_to_fnd(input, len);        // FND에 출력
    sleep(2);                        // 간단한 지연
}
