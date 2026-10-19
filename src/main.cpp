#include <Arduino.h>
#include <esp_task_wdt.h>

#define WDT_TIMEOUT_SEC 30

int ledPin = 5;

hw_timer_t* timer_work = NULL;

volatile int counter = 0;
int FULL_CYCLE_TIME = 3600;
int FAN_ON_TIME = 900;
int TICK = 1000000;  //  1 000 000 тіків = 1 секунда

void IRAM_ATTR onTimer() { counter++; }

void setup() {
    Serial.begin(115200);

    pinMode(ledPin, OUTPUT);

    digitalWrite(ledPin, LOW);

    timer_work = timerBegin(0, 80, true);

    // Прив'язка функції обробника;
    timerAttachInterrupt(timer_work, &onTimer, true);

    // Уставка алярму: 1 000 000 тіків = 1 секунда, увімкнено автоперезапуск (true)
    timerAlarmWrite(timer_work, TICK, true);

    // Активація апаратного переривання таймера
    timerAlarmEnable(timer_work);
}

void loop() {
    if (counter >= FAN_ON_TIME && counter < FULL_CYCLE_TIME) {
        digitalWrite(ledPin, LOW);
    }

    if (counter >= FULL_CYCLE_TIME) {
        counter = 0;
        digitalWrite(ledPin, HIGH);
    }
}
