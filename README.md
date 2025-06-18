![header](https://capsule-render.vercel.app/api?type=waving&color=_hexcode&height=200&section=header&text=We&nbsp;Are&nbsp;HKNU&nbsp;IoT&nbsp;TEAM&nbsp;5&nbsp;!&fontColor=FFFFFF&fontSize=60&animation=twinkling)

## Number Baseball Game 

이 프로젝트는 FPGA 모듈 기반의 숫자야구 게임입니다.  
Push Switch, FND, LED, Dot-Matrix, Text LCD, Buzzer 등을 활용하여 사용자와 AI가 번갈아 숫자를 맞추는 게임을 구현했습니다.

---

## Features

-  스위치를 통한 숫자 입력 및 중복 방지 처리
-  FND 실시간 입력 표시 및 정렬
-  Dot-Matrix 숫자 카운트다운 표시
-  LED를 이용한 남은 시도 횟수 시각화
-  Text LCD에 게임 상태, 결과 출력
-  Buzzer로 정답/실패/시간 초과 효과음 제공
-  난이도 설정 (Easy / Normal / Hard / AI 대결)
-  AI 자동 추측 모드 지원
-  게임 종료 후 요약 결과 표시 (횟수, 정답, 소요 시간)
-  Debug 모드: 터미널에 정답 출력 (테스트용)

---

## Hardware Modules Used

| 모듈         | 기능                                |
|--------------|-------------------------------------|
| Push Switch  | 숫자 입력 및 모드 선택                 |
| LED          | 시도 횟수 표시 (1개씩 꺼짐)            |
| FND          | 사용자 입력 숫자 실시간 표시            |
| Dot-Matrix   | 카운트다운 숫자 출력                   |
| Text LCD     | 턴/결과 메시지 출력                   |
| Buzzer       | 입력 피드백 / 성공 / 실패 알림          |

---

## Game Rules

- 사용자 또는 AI가 2자리 ~ 4자리 숫자를 추측합니다.
- 자리와 숫자가 모두 맞으면 **스트라이크(S)**  
- 숫자만 맞고 자리는 틀리면 **볼(B)**  
- 완전히 틀리면 **아웃(O)**  
- 8번 안에 정답을 맞히면 성공, 못 맞히면 실패 또는 무승부

---

## Difficulty Modes

| 모드       | 자리 수 | 제한 시간 |
|------------|---------|------------|
| Easy       | 2자리   | 9초        |
| Normal     | 3자리   | 9초        |
| Hard       | 4자리   | 5초        |
| AI 대결    | 3자리   | 9초        |

---

## How to Run

1. Raspberry Pi 또는 임베디드 보드에 `.c` 파일 업로드
2. 아래 명령어로 컴파일
   ```bash
   gcc -o number_baseball number_baseball_game.c
3. sudo ./number_baseball

## Branch Overview
- main : 전체 통합 및 디버그 정리본
- jeon : 숫자 입력 / FND 출력 / Dot-Matrix / 정답 판정 로직 등
- sabin : LCD 출력 / 난이도 선택 / 부저 효과음 / AI 모드 등

## Development Environment
- Raspberry Pi 4
- GCC (arm-linux-gcc)
- Linux Embedded Driver (/dev/fpga_XXX)
- Visual Studio Code (원격 SSH 개발)
- GitHub 버전 관리

