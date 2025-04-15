#include <Servo.h>
#include <AceRoutine.h>

using namespace ace_routine;

const int random_seed_pin = A0;
const int vr_in_pin = A1;
const int ldr_in_pin = A3;

const int data_pin = 2;
const int latch_pin = 3;
const int clock_pin = 4;
const int force_btn_pin = 8;
const int btn_pin = 9;
const int ldr_vcc_pin = 10;
const int vr_vcc_pin = 11;
const int servo_pin = 12;

int outcomeLEDs[4][2] = {
  {0, 4}, // 0 顆
  {1, 5}, // 1 顆
  {2, 6}, // 2 顆
  {3, 7}  // 再來一次
};

bool coin = false;
bool start = false;
long capsule_counter = 0;
long coin_counter = 0;
int finalLEDIndex = 0;
int outcome = 0; // 0: 0顆, 1: 1顆, 2: 2顆, 3: 再來一次
Servo motor;

// ====== 子函式 定義區 ======
void drop() {
  delay(1000);
  for (int i = 0; i <= 180; i++) {
    motor.write(i);
    delay(5);
  }
  delay(3000);
  for (int i = 180; i >= 0; i--) {
    motor.write(i);
    delay(5);
  }
}

void clearLEDs() {
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, 0);
  digitalWrite(latch_pin, HIGH);
}

// 設定單一 LED 點亮，ledIndex 為 0~7 （對應 LED 1~8）
void setLED(int ledIndex) {
  byte data = 1 << ledIndex;  // 產生只有該位元為1的資料
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, data);
  digitalWrite(latch_pin, HIGH);
}

COROUTINE(reward_led_blink) {
  COROUTINE_BEGIN();
  setLED(finalLEDIndex);
  COROUTINE_DELAY(300);
  clearLEDs();
  COROUTINE_DELAY(300);
  COROUTINE_END();
}

// 開始執行抽獎 LED 轉盤效果及結果決定
void runLotteryCycle() {
  int vr_percent = map(analogRead(vr_in_pin), 0, 1022, 0, 100);
  
  if (random(0, 100) < vr_percent) {
    outcome = 2;
    Serial.println("獎項決定：抽到 2 顆");
  } 
  else {
    // 由 0、1、3 隨機選一
    int r = random(0, 3);  // 可得到 0, 1, 2
    if (r == 0) {
      outcome = 0;
      Serial.println("獎項決定：抽到 0 顆");
    } 
    else if (r == 1) {
      outcome = 1;
      Serial.println("獎項決定：抽到 1 顆");
    } 
    else { // r == 2
      outcome = 3;
      Serial.println("獎項決定：再來一次");
    }
  }
  
  // 從 outcome 對應的 LED 位置中隨機選一個作為最終停留的 LED
  int finalLEDChoices[2] = { outcomeLEDs[outcome][0], outcomeLEDs[outcome][1] };
  finalLEDIndex = finalLEDChoices[random(0, 2)];
  Serial.print("最終停留 LED（以 0 為起始）： ");
  Serial.println(finalLEDIndex);
  
  // LED 轉盤效果：假設從 LED 0 開始
  int startIndex = 0;
  int cycles = 8;  // 最少迴圈數
  int offset = finalLEDIndex - startIndex;
  if (offset < 0) {
    offset += 8;
  }
  int totalSteps = cycles * 8 + offset;
  Serial.print("LED 轉盤總步數： ");
  Serial.println(totalSteps);
  
  // 轉盤效果的延遲設定：初始延遲及每步增加的時間 (ms)
  int delayTime = 15;
  int delayIncrement = 2;
  // 執行 LED 轉盤 (每步依序點亮下一顆 LED)
  for (int i = 0; i <= totalSteps; i++) {
    int currentLED = (startIndex + i) % 8;
    setLED(currentLED);
    delay(delayTime);
    delayTime += delayIncrement;
  }
  
  // 最後停留狀態保持一段時間
  // setLED(finalLEDIndex);
  for (int i = 0; i <= 3; i++) {
    reward_led_blink.runCoroutine();
  }

  // =======================================
  // 
  if (outcome == 0) {
    Serial.println("獎項判斷：抽到 0 顆");
    delay(1000);
  } 
  else if (outcome == 1){
    Serial.println("獎項判斷：抽到 1 顆");
    drop();
    delay(1000);
  }
  else if (outcome == 2){
    Serial.println("獎項判斷：抽到 2 顆");
    drop();
    delay(500);
    drop(); 
    delay(500);
  }
  else{
    Serial.println("獎項判斷：再抽一次");
    delay(1000);
    runLotteryCycle();
  }
  

  clearLEDs();
}

void pin_settings(){
  Serial.begin(9600);

  pinMode(latch_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);
  pinMode(btn_pin, INPUT);
  pinMode(ldr_vcc_pin, OUTPUT);
  pinMode(vr_vcc_pin, OUTPUT);

  digitalWrite(ldr_vcc_pin, HIGH);
  digitalWrite(vr_vcc_pin, HIGH);
  
  motor.attach(servo_pin);
  motor.write(0); // 歸0復位
  randomSeed(analogRead(A0));
  clearLEDs();
}

void led_idle_shift_1(){
  //Serial.println("led1");
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, 170);
  digitalWrite(latch_pin, HIGH);
}

void led_idle_shift_2(){
  //Serial.println("led2");
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, 85);
  digitalWrite(latch_pin, HIGH);
}

void led_coin_show(){
  //Serial.println("led_all_show");
  digitalWrite(latch_pin, LOW);
  shiftOut(data_pin, clock_pin, LSBFIRST, 255);
  digitalWrite(latch_pin, HIGH);
}

// ====== coroutine 定義區 ======

COROUTINE(idle) {
  COROUTINE_LOOP(){
    if (!coin) {
      // led show
      led_idle_shift_1();
      COROUTINE_DELAY(500);
      led_idle_shift_2();
      COROUTINE_DELAY(500);
    }
  }
}

COROUTINE(flash_led) {
  COROUTINE_LOOP(){
    led_coin_show();
    COROUTINE_DELAY(500);
    clearLEDs();
    COROUTINE_DELAY(500);
  }
}

COROUTINE(detect_coin) {
  COROUTINE_LOOP(){
    if (analogRead(ldr_in_pin) > 500){
      Serial.println("====================");
      Serial.println("投幣");
      coin = true;
    }
    
    while(coin){
      flash_led.runCoroutine(); //wierd?

      if(digitalRead(btn_pin) == HIGH){
        Serial.println("解開thread");
        start = true;
        Serial.println("按下按鈕");
        runLotteryCycle();
        Serial.println("====================");
        break;
      }
    }
    
    coin = false;
    start = false;
    COROUTINE_YIELD(); // thread handsoff
  }
}

// ====== 主程式 ======

void setup() {
  pin_settings();
}

void loop() {
  idle.runCoroutine();
  detect_coin.runCoroutine();
}
