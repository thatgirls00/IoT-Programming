#include <fcntl.h>
#include <unistd.h>

#define LED_DEVICE "/dev/fpga_led"

// [LED 출력 함수]
// 시도 횟수만큼 LED를 끄고, 나머지 LED는 켜짐 상태 유지
void write_to_led(int tries) {
    int fd = open(LED_DEVICE, O_WRONLY);
    if (fd < 0) return;

    // 최대 8번까지 시도 가능, 왼쪽부터 꺼짐
    // ex) tries = 3 -> LED = 0b00011111 (왼쪽 3개 off)
    unsigned char led = (tries > 8) ? 0x00 : 0xFF << tries;
    write(fd, &led, 1);
    close(fd);
}
