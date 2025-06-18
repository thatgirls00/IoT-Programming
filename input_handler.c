// [1] 숫자 입력 로직 및 중복 방지
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
}
