// [2] FND에 입력 숫자 정렬 표시
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
