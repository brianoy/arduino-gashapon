// 這個程式是拿來測試所有的電子元件都正常連接和運作

void setup() {
  // 設定數位腳位為輸入
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  // 啟用序列通訊
  Serial.begin(9600);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
}

void loop() {

  for (int i = 0; i < 8; i++) {
    // 逐一點亮 LED
    // 讀取數位腳位狀態

    int state8 = digitalRead(8);
    int state9 = digitalRead(9);
    int analogA1 = analogRead(A1);
    int analogA3 = analogRead(A3);

    // 輸出所有讀數到序列視窗
    Serial.print("綠色按鈕: ");
    Serial.print(state8 == HIGH ? "HIGH" : "LOW");
    Serial.print(" | 藍色按鈕: ");
    Serial.print(state9 == HIGH ? "HIGH" : "LOW");
    Serial.print(" | 可變電阻: ");
    Serial.print(analogA1);
    Serial.print(" | 光敏電阻: ");
    Serial.println(analogA3);

    byte data = 1 << i;
    digitalWrite(3, LOW);
    shiftOut(2, 4, MSBFIRST, data);
    digitalWrite(3, HIGH);
    delay(analogA1); //delay 30 最快 逐漸變慢
  }

}
