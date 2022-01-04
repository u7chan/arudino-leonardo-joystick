#include "Joystick.h"

#define MAX_SW 4                  // 最大スイッチ数
#define CHATTERING_THRESHOLD 100  // チャタリング防止の閾値

const int PIN_SW[MAX_SW] = {8, 6, 4, 2};  // スイッチのピン番号
int sw_cnt[MAX_SW];

void setup() {
    for (int i = 0; i < MAX_SW; ++i) {
        pinMode(PIN_SW[i], INPUT);
    }
    Joystick.begin();
}

void loop() {
    static int prevBits = 0;
    int inputBits = 0;
    for (int i = 0; i < MAX_SW; ++i) {
        inputBits |= (update(PIN_SW[i], &sw_cnt[i]) << (i * 1));
    }
    if (inputBits != prevBits) {
        prevBits = inputBits;
        dispatchEvent(inputBits);
    }
}

byte update(int pin, int *cnt) {
    if (digitalRead(pin) == HIGH) {
        ++(*cnt);
        *cnt = min(*cnt, CHATTERING_THRESHOLD);
    } else {
        *cnt = 0;
    }
    if (*cnt >= CHATTERING_THRESHOLD) {
        return 0x01;  // PUSH
    }
    return 0x00;  // IDLE
}

void dispatchEvent(int inputBits) {
    for (int i = 0; i < MAX_SW; ++i) {
        int swBits = (inputBits >> i) & 0x01;
        if (swBits) {
            Joystick.pressButton(i);
        } else {
            Joystick.releaseButton(i);
        }
    }
}
