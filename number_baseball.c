#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define SWITCH_DEVICE "/dev/fpga_push_switch"
#define LCD_DEVICE    "/dev/fpga_text_lcd"
#define LED_DEVICE    "/dev/fpga_led"
#define FND_DEVICE    "/dev/fpga_fnd"
#define DOT_DEVICE    "/dev/fpga_dot"
#define BUZZER_DEVICE "/dev/fpga_buzzer"

unsigned char dot_patterns[10][8] = {
    {0x3C,0x66,0x6E,0x76,0x66,0x66,0x3C,0x00}, // 0
    {0x18,0x38,0x18,0x18,0x18,0x18,0x3C,0x00}, // 1
    {0x3C,0x66,0x06,0x1C,0x30,0x66,0x7E,0x00}, // 2
    {0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00}, // 3
    {0x0C,0x1C,0x3C,0x6C,0x7E,0x0C,0x0C,0x00}, // 4
    {0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00}, // 5
    {0x3C,0x66,0x60,0x7C,0x66,0x66,0x3C,0x00}, // 6
    {0x7E,0x66,0x0C,0x18,0x18,0x18,0x18,0x00}, // 7
    {0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00}, // 8
    {0x3C,0x66,0x66,0x3E,0x06,0x66,0x3C,0x00}  // 9
};

void buzz(int duration_ms) {
    int fd = open(BUZZER_DEVICE, O_WRONLY);
    if (fd < 0) return;
    unsigned char on = 1;
    write(fd, &on, 1);
    usleep(duration_ms * 1000);
    unsigned char off = 0;
    write(fd, &off, 1);
    close(fd);
}

void buzz_success() { for (int i = 0; i < 3; i++) { buzz(100); usleep(100000); } }
void buzz_fail()    { buzz(600); }
void buzz_timeout() { for (int i = 0; i < 2; i++) { buzz(200); usleep(100000); } }
void buzz_input()   { buzz(80); }

void lcd_write_message(const char* msg) {
    int fd = open(LCD_DEVICE, O_WRONLY);
    if (fd < 0) return;
    char line[33] = {0};
    snprintf(line, sizeof(line), "%-32s", msg);
    write(fd, line, 32);
    close(fd);
}

void write_turn_and_result_to_lcd(const char* turn, int strike, int ball, int out) {
    int fd = open(LCD_DEVICE, O_WRONLY);
    if (fd < 0) return;

    char upper[17] = {0};
    char lower[17] = {0};
    char full[33] = {0};

    snprintf(upper, sizeof(upper), "%-16s", turn); // 상단: 'User Turn' 등
    snprintf(lower, sizeof(lower), "S:%d B:%d O:%d   ", strike, ball, out); // 하단

    // 전체 32자 채워서 LCD에 출력
    snprintf(full, sizeof(full), "%-16s%-16s", upper, lower);
    write(fd, full, 32);
    close(fd);
}

void write_game_summary_to_lcd(int tries, int* answer, int len, const char* result, time_t elapsed_sec) {
    int fd = open(LCD_DEVICE, O_WRONLY);
    if (fd < 0) return;
    char line[33] = {0};
    char upper[17] = {0}, lower[17] = {0};

    snprintf(upper, sizeof(upper), "%s T:%d %lds", result, tries, elapsed_sec);
    snprintf(lower, sizeof(lower), "Ans:");
    for (int i = 0; i < len; i++) {
        char num[4];
        snprintf(num, sizeof(num), "%d ", answer[i]);
        strncat(lower, num, sizeof(lower) - strlen(lower) - 1);
    }

    snprintf(line, sizeof(line), "%-16s%-16s", upper, lower);
    write(fd, line, 32);
    close(fd);
}

void write_to_led(int tries) {
    int fd = open(LED_DEVICE, O_WRONLY);
    if (fd < 0) return;
    unsigned char led = (tries > 8) ? 0x00 : 0xFF << tries;
    write(fd, &led, 1);
    close(fd);
}

void write_to_fnd(int* input, int len) {
    int fd = open(FND_DEVICE, O_WRONLY);
    if (fd < 0) return;
    unsigned char fnd_data[4] = {0};
    for (int i = 0; i < len && i < 4; i++)
        if (input[i] != -1)
            fnd_data[4 - len + i] = input[i];
    write(fd, fnd_data, 4);
    close(fd);
}

void write_dot_number(int num) {
    if (num < 0 || num > 9) return;
    int fd = open(DOT_DEVICE, O_WRONLY);
    if (fd < 0) return;
    write(fd, dot_patterns[num], 8);
    close(fd);
}

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

int read_single_switch() {
    int fd = open(SWITCH_DEVICE, O_RDONLY);
    if (fd < 0) return -1;
    unsigned char buf[9];
    read(fd, buf, sizeof(buf));
    close(fd);
    for (int i = 0; i < 9; i++)
        if (buf[i] == 1) return i + 1;
    return -1;
}

void ai_guess_number(int* guess, int len) {
    int used[10] = {0}, count = 0;
    while (count < len) {
        int num = (rand() % 9) + 1;
        if (!used[num]) {
            guess[count++] = num;
            used[num] = 1;
        }
    }
}

int select_mode() {
    lcd_write_message("1.Easy 2.Normal 3.Hard 4.AI");
    printf("난이도를 선택하세요: 1~4번\n");
    int mode = 0;
    while (1) {
        mode = read_single_switch();
        if (mode >= 1 && mode <= 4) {
            buzz_input();
            return mode;
        }
        usleep(200000);
    }
}

int main() {
    srand(time(NULL));

    int mode = select_mode();
    int len = (mode == 1) ? 2 : (mode == 3) ? 4 : 3;
    int timeout = (mode == 3) ? 5 : 9;
    int ai_mode = (mode == 4);
    if (ai_mode) len = 3;

    int answer[4];
    generate_answer(answer, len);

    // ⬇️ 정답 터미널 출력 (추가)
    printf("정답 (Debug Only): ");
    for (int i = 0; i < len; i++) {
        printf("%d ", answer[i]);
    }
    printf("\n");

    int tries = 0;
    int input[4] = {-1, -1, -1, -1};
    int input_count;
    int first_turn = 1;

    lcd_write_message("Game Start");
    printf("게임을 시작합니다! 스위치를 눌러 숫자를 입력하세요 (1~9).\n");
    sleep(2);

    time_t game_start = time(NULL);

    while (1) {
        if (first_turn) {
            lcd_write_message("Input numbers");
            first_turn = 0;
        }

        input_count = 0;
        memset(input, -1, sizeof(input));
        time_t start_time = time(NULL);
        int timed_out = 0;

        if (!ai_mode || (tries % 2 == 0)) {
            write_turn_and_result_to_lcd("User Turn", 0, 0, 0);
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
            write_turn_and_result_to_lcd("AI Turn", 0, 0, 0);
            ai_guess_number(input, len);
            printf("[AI 입력]: ");
            for (int i = 0; i < len; i++) printf("%d ", input[i]);
            printf("\n");
            write_to_fnd(input, len);
            sleep(2);
        }

        if (timed_out) {
            lcd_write_message("⏱ Time Out!");
            printf("시간 초과! 게임 종료\n");
            buzz_timeout();
            time_t game_end = time(NULL);
            write_game_summary_to_lcd(tries, answer, len, "Fail", game_end - game_start);
            return 0;
        }

        int strike, ball, out;
        compare_positions(answer, input, &strike, &ball, &out, len);
        tries++;
        write_to_led(tries);
        write_turn_and_result_to_lcd((!ai_mode || tries % 2 == 1) ? "User Turn" : "AI Turn", strike, ball, out);
        printf("결과 - S:%d B:%d O:%d | T:%d\n", strike, ball, out, tries);

        if (strike == len) {
            const char* winner = ai_mode ? ((tries % 2 == 1) ? "🎉 USER WIN!" : "🤖 AI WIN!") : "🎉 YOU WIN!";
            lcd_write_message(winner);
            printf("%s\n", winner);
            buzz_success();
            time_t game_end = time(NULL);
            write_game_summary_to_lcd(tries, answer, len, ai_mode ? ((tries % 2 == 1) ? "USER" : "AI") : "Win", game_end - game_start);
            sleep(3);
            return 0;
        }

        if (tries >= 8) {
            const char* result = ai_mode ? "🤝 DRAW!" : "💀 Game Over";
            lcd_write_message(result);
            printf("%s\n", result);
            buzz_fail();
            time_t game_end = time(NULL);
            write_game_summary_to_lcd(tries, answer, len, ai_mode ? "Draw" : "Fail", game_end - game_start);
            sleep(3);
            return 0;
        }

        sleep(1);
    }

    return 0;
}
