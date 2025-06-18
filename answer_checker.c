// [4-1] 정답 난수 생성
void generate_answer(int* answer, int len) {
    int used[10] = {0}, count = 0;
    while (count < len) {
        int num = (rand() % 9) + 1;
        if (!used[num]) {
            answer[count++] = num;
            used[num] = 1;
        }
    }
}

// [4-2] 사용자 입력과 정답 비교 (S/B/O 판정)
void compare_positions(int* answer, int* guess, int* strike, int* ball, int* out, int len) {
    *strike = *ball = *out = 0;
    for (int i = 0; i < len; i++) {
        if (guess[i] == answer[i]) (*strike)++;
        else {
            int found = 0;
            for (int j = 0; j < len; j++)
                if (i != j && guess[i] == answer[j]) found = 1;
            if (found) (*ball)++;
            else (*out)++;
        }
    }
}
