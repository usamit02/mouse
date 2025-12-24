#include <BleMouse.h>
#define joyX_PIN  4    // X軸(横軸)
#define joyY_PIN  2    // 7軸(縦軸)
#define joySw_PIN 5    // スイッチ
int xx = 0, yy = 0, sww;
int posX = 90, posY, center = 0;
int input;
int count = 0;
BleMouse bleMouse("esp32 mouse");
void setup() {
  Serial.begin(115200);
  pinMode(joyX_PIN, INPUT);
  pinMode(joyY_PIN, INPUT);
  pinMode(joySw_PIN, INPUT_PULLUP);
  bleMouse.begin();
  delay(300);
}

void loop() {
  int joyX  = analogRead(joyX_PIN);
  int joyY  = analogRead(joyY_PIN);
  int joySw = digitalRead(joySw_PIN);
  if (bleMouse.isConnected()) {
    if (joySw == 1) {    // スイッチを離したとき
      // Serial.println("OFF");
    } else {    // スイッチを押したとき
      bleMouse.click(MOUSE_LEFT);
      Serial.println("ON");
    }
    int x, y, centerX = 2340, centerY = 2754;
    int threshold = 200;    // 個体差を吸収するために閾値を用意する
    x             = joyX - centerX;
    y             = joyY - centerY;
    if (x > threshold || y > threshold || x < -threshold || y < -threshold) {
      if (joyX > centerX) {
        y = map(joyX, centerX, 4095, 0, 10);

      } else {
        y = map(joyX, centerX, 0, 0, -10);
      }
      y = joyX > centerX ? map(joyX, centerX, 4095, 0, 10) : map(joyX, centerX, 0, 0, -10);
      x = joyY > centerY ? map(joyY, centerY, 4095, 0, 10) : map(joyY, centerY, 0, 0, -10);
      if (y < 8 && y > -8 || y > 0 && yy < 0 || y < 0 && yy > 0) {
        yy = 0;
      } else {
        yy += (y > 8 && yy >= 0 && yy < 150) - (y < 8 && yy <= 0 && yy > -150);
      }
      if (x < 8 && x > -8 || x > 0 && xx < 0 || x < 0 && xx > 0) {
        xx = 0;
      } else {
        xx += ((x > 8 && xx >= 0 && xx < 150) - (x < 8 && xx <= 0 && xx > -150)) * 2;
      }
      bleMouse.move(x + xx, y + yy, 0);
      if (count > 5) {
        count = 0;
        Serial.println("x=" + String(x) + "y=" + String(y) + "yy=" + String(yy));
      }
      count++;
    }
  }
  delay(50);
}