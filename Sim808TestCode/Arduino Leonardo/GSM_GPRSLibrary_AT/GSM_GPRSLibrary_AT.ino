void setup() {
  Serial.begin(9600); //初始化Serial（即USB串口）
  Serial1.begin(9600);//初始化Serial1
}

void loop() {
  while (Serial1.available()) {
    Serial.write(Serial1.read());//如果Serial1收到数据则通过Serial输出
  }
  while (Serial.available()) {
    Serial1.write(Serial.read());//如果Serial收到数据则通过Serial1输出
  }
  delay(1);//短暂延时，避免USB-COM不稳定
}
