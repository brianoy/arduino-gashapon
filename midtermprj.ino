#include <Servo.h>
#include <AceRoutine.h>

using namespace ace_routine;

const int random_seed_pin = A0;
const int vr_in_pin = A1;
const int ldr_in_pin = A3;

const int data_pin = 2;
const int latch_pin = 3;
const int clock_pin = 4;
const int NO_CONNECT_1 = 7;
const int force_btn_pin = 8;
const int btn_pin = 9;
const int ldr_vcc_pin = 10;
const int vr_vcc_pin = 11;
const int servo_pin = 12;
const int NO_CONNECT_2 = 13;

int outcomeLEDs[4][2] = {
  {0, 4}, // 0 顆 黃色
  {1, 5}, // 1 顆 綠色
  {2, 6}, // 2 顆 紅色 大獎
  {3, 7}  // 再來一次 白色
};

// 狀態變數
int outcome = -1; // 0: 0顆, 1: 1顆, 2: 2顆, 3: 再來一次 -1:NULL
int capsule_buffer = 0;
int stage = 0;

int capsule_counter = 0;
int coin_counter = 0;
int finalLEDIndex = 0;

static int i = 0;
static int j = 0;

Servo motor;

// ====== 子函式 定義區 ======
void clearLEDs() {
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, 0);
  digitalWrite(latch_pin, HIGH);
}

COROUTINE(step5) {
  COROUTINE_LOOP(){
    if (capsule_buffer > 0){
      // bug: for 內不能使用區域變數
      static int j = 0;
      for (j = 0; j <= 180; j++) {
        motor.write(j);
        COROUTINE_DELAY(5);
      }
      COROUTINE_DELAY(2500);
      for (j = 180; j >= 0; j--) {
        motor.write(j);
        COROUTINE_DELAY(5);
      }
      COROUTINE_DELAY(2500);
      capsule_counter++;
      capsule_buffer--;
      COROUTINE_YIELD();
    }
    else if (stage == 5 && capsule_buffer == 0) {
      stage = 6; // bugfix: 不存在的stage 這樣後面就不會閃爍衝突
      clearLEDs();
      COROUTINE_DELAY(1000);
      Serial.print("總扭出數量: ");
      Serial.print(capsule_counter);
      Serial.print(" 總投幣量: ");
      Serial.print(coin_counter);
      Serial.println();
      Serial.println("==========(結束)==========");
      stage = 0; // 復位
      COROUTINE_YIELD();
    }
    else{
      COROUTINE_YIELD();
    }
  }
}

// 設定單一 LED 點亮，ledIndex 為 0~7 （對應 LED 1~8）
void setLED(int ledIndex) {
  byte data = 1 << ledIndex;  // 產生只有該位元為1的資料
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, data);
  digitalWrite(latch_pin, HIGH);
}

COROUTINE(reward_led_blink) {
  COROUTINE_LOOP(){
    if (stage == 5 || stage == 4){
      setLED(finalLEDIndex);
      COROUTINE_DELAY(300);
      clearLEDs();
      COROUTINE_DELAY(300);
    }
    else{
      COROUTINE_YIELD();
    }
  }
}

COROUTINE(step4) {
  COROUTINE_LOOP() {
    if (stage == 4){
      COROUTINE_DELAY(1500);
      if (outcome == 0) {
        Serial.println("獎項判斷：抽到 0 顆");
        COROUTINE_DELAY(2000); // bug fix:這樣才可以觸發閃爍
        stage = 5;
      }
      else if (outcome == 1){
        Serial.println("獎項判斷：抽到 1 顆");
        capsule_buffer++;
        stage = 5;
      }
      else if (outcome == 2){
        Serial.println("獎項判斷：抽到 2 顆");
        capsule_buffer++;
        capsule_buffer++;
        stage = 5;
      }
      else{
        Serial.println("獎項判斷：再抽一次");
        Serial.println("-->");
        COROUTINE_DELAY(3000); // spec: 要三秒
        stage = 3;
      }
      outcome = -1;
    }
    COROUTINE_YIELD();
  }
}

void choose_reward(int chance) { // fast
  if (random(0, 100) < chance) {
    // 機率為x
    outcome = 2;
    Serial.print("機率: ");
    Serial.print(chance);
    Serial.println("，獎項決定：抽到 2 顆");
  } 
  else {
    // feat: 降低再來一次的機率，機率顯示

    // 由 0、1、3 隨機選一
    int r = random(0, 5);  // 可得到 0, 1, 2,3,4
    if (r == 0 || r == 1) {
      // 機率0.4*(1-x)
      outcome = 0;
      Serial.print("機率: ");
      Serial.print(0.4*(100-chance));
      Serial.println("，獎項決定：抽到 0 顆");
    } 
    else if (r == 2 || r == 3) {
      // 機率0.4*(1-x)
      outcome = 1;
      Serial.print("機率: ");
      Serial.print(0.4*(100-chance));
      Serial.println("，獎項決定：抽到 1 顆");
    } 
    else { // r == 4
      // 機率0.2*(1-x)
      outcome = 3;
      randomSeed(random_seed_pin); // feat: 變更隨機seed
      Serial.print("機率: ");
      Serial.print(0.2*(100-chance));
      Serial.println("，獎項決定：再來一次");
    }
  }
}

void pin_settings(){
  Serial.begin(9600);

  pinMode(latch_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);
  pinMode(btn_pin, INPUT);
  pinMode(ldr_vcc_pin, OUTPUT);
  pinMode(vr_vcc_pin, OUTPUT);
  // ====================
  digitalWrite(ldr_vcc_pin, HIGH);
  digitalWrite(vr_vcc_pin, HIGH);
  // ====================
  motor.attach(servo_pin);
  motor.write(0); // motor試轉
  delay(100);
  motor.write(10); // motor試轉
  delay(300);
  motor.write(0); // motor試轉
  delay(100);
  motor.write(10); // motor試轉
  delay(100);
  motor.write(0);// motor歸0復位
  
  randomSeed(random_seed_pin);
  clearLEDs();
}

void led_idle_shift_1(){
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, 170);
  digitalWrite(latch_pin, HIGH);
}

void led_idle_shift_2(){
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, 85);
  digitalWrite(latch_pin, HIGH);
}

void led_coin_show(){
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, 255);
  digitalWrite(latch_pin, HIGH);
}

COROUTINE(step0) {
  COROUTINE_LOOP() {
    if (stage == 0) {
      // led show
      led_idle_shift_1();
      COROUTINE_DELAY(500);
      led_idle_shift_2();
      COROUTINE_DELAY(500);
    }
    else{ // 沒有要閃燈就handsoff
      COROUTINE_YIELD();
    }
  }
}

COROUTINE(flash_all_led) {
  COROUTINE_LOOP() {
    if (stage == 1){ // 透下硬幣但是還沒開始
      led_coin_show();
      COROUTINE_DELAY(500);
      clearLEDs();
      COROUTINE_DELAY(500);
    }
    else{ // 沒有要閃燈就handsoff
      COROUTINE_YIELD();
    }
  }
}

COROUTINE(step1) {
  COROUTINE_LOOP(){
    if ((stage == 0) && analogRead(ldr_in_pin) > 500){
      Serial.println("==========(開始)==========");
      Serial.println("投幣");
      stage = 1;
      coin_counter++;
    }
    
    if ((stage == 1) && (digitalRead(btn_pin) == HIGH)) {
      while (digitalRead(btn_pin) == HIGH); // bugfix: 防按鈕連擊
      stage = 2;
      Serial.println("按下按鈕");
      delay(50); // bugfix: 防止跳過step3 main thread block
    }
    COROUTINE_YIELD(); // handsoff
  }
}

COROUTINE(force_btn) {
  COROUTINE_LOOP(){
    if ((stage == 0) && (digitalRead(force_btn_pin) == HIGH)){
      stage = 5;
      capsule_buffer = 1; // 退出一顆
    }
    COROUTINE_YIELD(); // handsoff
  }
}

COROUTINE(step2) {
  COROUTINE_LOOP(){
    if (stage == 2 || stage == 3){
      int vr_percent = map(analogRead(vr_in_pin), 20, 1000, 0, 100); // 抽到兩顆的機率 我這邊VR有問題所以是1021 正常是1023
      choose_reward(vr_percent);

      // 從 outcome 對應的 LED 位置中隨機選一個作為最終停留的 LED
      int finalLEDChoices[2] = { outcomeLEDs[outcome][0], outcomeLEDs[outcome][1] };
      finalLEDIndex = finalLEDChoices[random(0, 2)];
      Serial.print("最終停留 LED（以 0 為起始）： ");
      Serial.println(finalLEDIndex);
      
      // LED 轉盤效果：假設從 LED 0 開始
      int startIndex = 0;
      // ===================================
      int delayTime = 30; // 轉盤效果初始延遲 
      int delayIncrement = 2; // 轉盤效果每步增加的時間
      int currentLED = 0;

      if (stage == 2) {
        for (i = 0; i <= 300; i++) {
          currentLED = (startIndex + i) % 8;
          setLED(currentLED);
          delay(delayTime);
          if (digitalRead(btn_pin) == HIGH) {
            // while (digitalRead(btn_pin) == HIGH); // bugfix: 防按鈕連擊 // 這邊好像不需要了
            stage = 3;
            startIndex = currentLED;
            break; // stage3
          }
        }
      }

      int offset = finalLEDIndex - startIndex;
      if (offset < 0) {
        offset += 8;
      }
      int totalSteps = 8 * 8 + offset;
      Serial.print("LED 轉盤總步數： ");
      Serial.println(totalSteps);

      stage = 3;

      for (i = 0; i <= totalSteps; i++) {
        currentLED = (startIndex + i) % 8;
        setLED(currentLED);
        delay(delayTime);
        delayTime += delayIncrement;
      }
      stage = 4;
    }
    COROUTINE_YIELD(); // handsoff
  }
}


// ====== 主程式 ======

void setup() {
  pin_settings();
}

void loop() {
  step0.runCoroutine();
  step1.runCoroutine();
  flash_all_led.runCoroutine();
  step2.runCoroutine();
  step4.runCoroutine();
  reward_led_blink.runCoroutine();
  step5.runCoroutine();
  force_btn.runCoroutine();
}
