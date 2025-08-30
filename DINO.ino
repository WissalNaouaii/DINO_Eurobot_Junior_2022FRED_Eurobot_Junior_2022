#include <Wire.h>
#include <LiquidCrystal_I2C.h> 
#include <TimerOne.h>
#include <PS3BT.h>
#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
#include <Servo.h>
#define motorUp 7
#define motorDown 8
#define FinCourse A8
USB Usb;
BTD Btd(&Usb);
PS3BT PS3(&Btd);
bool printTemperature, printAngle;
//----- Adressage matériel -----
// En cas de non fonctionnement, mettez la ligne 8 en
// commentaire et retirez le commentaire à la ligne 9.
//pin motor
int vit=150;
int vit1=150;
int MotorSpeedB, MotorSpeedA;
int SpeedLeft, SpeedRight;
int rot_speed;
int base_speed;
int val1 = 1;
char val;
int led_red_pin = 34;
int led_bleu_pin = 38;
int led_orange_pin = 36;
int test_pin = A11;
int md1 = 2;
int md2 = 3;
int mg1 = 4;
int mg2 = 5;
int b_pin = A0;
int battery_input_max = 488; //650(16v)
float currentE, prevE, diffE, sommeE, initalError = 0;
float consigne = 0;
float Kp = 6;
float Ki = 0.1;
float Kd = 5;
int cmd;
//variable encodeur
float precision = 4;
int encoderRA = 18;
int encoderRB = 19;
int encoderRPos = 0;
int lastEncoderRPos = 0;
boolean encoderRA_set = HIGH;
boolean encoderRB_set = HIGH;
//variable systeme vis ecrou
int pas_apparant = 5;
int n_filet = 4;
float dist;
float encoder_ticks = 224.5;
float dist_par_tour = (float)(n_filet*pas_apparant);
float distT = dist_par_tour / (encoder_ticks*precision);
Servo bala;
Servo r_pince;
Servo l_pince;
Servo servo1;
Servo servo2;
Servo res;
Servo hakeka;
int hakeka_pin = 48;
int bala_pin = 46;
int r_pince_pin = 45;
int l_pince_pin = 47;
int servo1Pin = 49;
int servo2Pin = 42;
int res_pin = 43;
int balapos = 120;
int l_pincepos = 0;
int r_pincepos = 150;
int respos = 90;
int hakekapos = 100;
//
LiquidCrystal_I2C lcd(0x27, 20, 4);
TimerOne timer;
int score_total = 0;
int remaining_time = 100;
bool started = false;
bool game_over = false;
//les nombres
byte tiret_bas[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
};
byte tiret_haut[8] = {
  0b11111,
  0b11111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
void print0(int col) {
  lcd.setCursor(col, 0);
  lcd.write(255);
  lcd.setCursor(col + 1, 0);
  lcd.write(255);
  lcd.setCursor(col + 2, 0);
  lcd.write(255);
  lcd.setCursor(col, 1);
  lcd.write(255);
  lcd.setCursor(col + 2, 1);
  lcd.write(255);
  lcd.setCursor(col, 2);
  lcd.write(255);
  lcd.setCursor(col + 2, 2);
  lcd.write(255);
  lcd.setCursor(col, 3);
  lcd.write(255);
  lcd.setCursor(col + 1, 3);
  lcd.write(255);
  lcd.setCursor(col + 2, 3);
  lcd.write(255);
}
void print1(int col) {
  lcd.setCursor(col + 1, 0);
  lcd.write(255);
  lcd.setCursor(col, 1);
  lcd.write(255);
  lcd.setCursor(col + 1, 1);
  lcd.write(255);
  lcd.setCursor(col + 1, 2);
  lcd.write(255);
  lcd.setCursor(col, 3);
  lcd.write(255);
  lcd.setCursor(col + 1, 3);
  lcd.write(255);
  lcd.setCursor(col + 2, 3);
  lcd.write(255);
}
void print2(int col) {
  lcd.setCursor(col, 0);
  lcd.write(255);
  lcd.setCursor(col + 1, 0);
  lcd.write(255);
  lcd.setCursor(col + 2, 0);
  lcd.write(255);
  lcd.setCursor(col + 2, 1);
  lcd.write(255);
  lcd.createChar(0, tiret_bas);
  lcd.setCursor(col + 1, 1);
  lcd.write(0);
  lcd.createChar(0, tiret_bas);
  lcd.setCursor(col, 1);
  lcd.write(0);
  lcd.createChar(1, tiret_haut);
  lcd.setCursor(col + 1, 2);
  lcd.write(1);
  lcd.createChar(1, tiret_haut);
  lcd.setCursor(col + 2, 2);
  lcd.write(1);
  lcd.setCursor(col, 2);
  lcd.write(255);
  lcd.setCursor(col, 3);
  lcd.write(255);
  lcd.setCursor(col + 1, 3);
  lcd.write(255);
  lcd.setCursor(col + 2, 3);
  lcd.write(255);;
}
void print3(int col) {
  lcd.setCursor(col, 0);
  lcd.write(255);
  lcd.setCursor(col + 1, 0);
  lcd.write(255);
  lcd.setCursor(col + 2, 0);
  lcd.write(255);
  lcd.setCursor(col + 2, 1);
  lcd.write(255);
  lcd.createChar(0, tiret_bas);
  lcd.setCursor(col + 1, 1);
  lcd.write(0);
  lcd.createChar(0, tiret_bas);
  lcd.setCursor(col, 1);
  lcd.write(0);
  lcd.createChar(1, tiret_haut);
  lcd.setCursor(col + 1, 2);
  lcd.write(1);
  lcd.createChar(1, tiret_haut);
  lcd.setCursor(col, 2);
  lcd.write(1);
  lcd.setCursor(col + 2, 2);
  lcd.write(255);
  lcd.setCursor(col, 3);
  lcd.write(255);
  lcd.setCursor(col + 1, 3);
  lcd.write(255);
  lcd.setCursor(col + 2, 3);
  lcd.write(255);
}
void print4(int col) {
  lcd.setCursor(col + 2, 0);
  lcd.write(255);
  lcd.setCursor(col + 2, 1);
  lcd.write(255);
  lcd.setCursor(col + 2, 2);
  lcd.write(255);
  lcd.setCursor(col + 2, 3);
  lcd.write(255);
  lcd.createChar(0, tiret_bas);
  lcd.write(0);
  lcd.setCursor(col, 0);
  lcd.write(255);
  lcd.setCursor(col, 1);
  lcd.write(255);
  lcd.createChar(1, tiret_haut);
  lcd.setCursor(col, 2);
  lcd.write(1);
  lcd.createChar(1, tiret_haut);
  lcd.setCursor(col + 1, 2);
  lcd.write(1);
}
void print5(int col) {
  lcd.setCursor(col, 0);
  lcd.write(255);
  lcd.setCursor(col + 1, 0);
  lcd.write(255);
  lcd.setCursor(col + 2, 0);
  lcd.write(255);
  lcd.setCursor(col, 1);
  lcd.write(255);
  lcd.createChar(0, tiret_bas);
  lcd.setCursor(col + 1, 1);
  lcd.write(0);
  lcd.createChar(0, tiret_bas);
  lcd.setCursor(col + 2, 1);
  lcd.write(0);
  lcd.createChar(1, tiret_haut);
  lcd.setCursor(col + 1, 2);
  lcd.write(1);
  lcd.createChar(1, tiret_haut);
  lcd.setCursor(col, 2);
  lcd.write(1);
  lcd.setCursor(col + 2, 2);
  lcd.write(255);
  lcd.setCursor(col, 3);
  lcd.write(255);
  lcd.setCursor(col + 1, 3);
  lcd.write(255);
  lcd.setCursor(col + 2, 3);
  lcd.write(255);
}
void print6(int col) {
  lcd.setCursor(col, 0);
  lcd.write(255);
  lcd.setCursor(col + 1, 0);
  lcd.write(255);
  lcd.setCursor(col + 2, 0);
  lcd.write(255);
  lcd.setCursor(col, 1);
  lcd.write(255);
  lcd.createChar(0, tiret_bas);
  lcd.setCursor(col + 1, 1);
  lcd.write(0);
  lcd.createChar(0, tiret_bas);
  lcd.setCursor(col + 2, 1);
  lcd.write(0);
  lcd.setCursor(col + 2, 2);
  lcd.write(255);
  lcd.setCursor(col, 3);
  lcd.write(255);
  lcd.setCursor(col + 1, 3);
  lcd.write(255);
  lcd.setCursor(col + 2, 3);
  lcd.write(255);
  lcd.setCursor(col, 2);
  lcd.write(255);
}
void print7(int col) {
  lcd.setCursor(col, 0);
  lcd.write(255);
  lcd.setCursor(col + 1, 0);
  lcd.write(255);
  lcd.setCursor(col + 2, 0);
  lcd.write(255);
  lcd.setCursor(col + 2, 1);
  lcd.write(255);
  lcd.setCursor(col + 1, 2);
  lcd.write(255);
  lcd.setCursor(col, 3);
  lcd.write(255);
}
void print8(int col) {
  lcd.setCursor(col, 0);
  lcd.write(255);
  lcd.setCursor(col + 1, 0);
  lcd.write(255);
  lcd.setCursor(col + 2, 0);
  lcd.write(255);
  lcd.setCursor(col, 1);
  lcd.write(255);
  lcd.createChar(0, tiret_bas);
  lcd.setCursor(col + 1, 1);
  lcd.write(0);
  lcd.setCursor(col + 2, 1);
  lcd.write(255);
  lcd.setCursor(col, 2);
  lcd.write(255);
  lcd.createChar(1, tiret_haut);
  lcd.setCursor(col + 1, 2);
  lcd.write(1);
  lcd.setCursor(col + 2, 2);
  lcd.write(255);
  lcd.setCursor(col, 3);
  lcd.write(255);
  lcd.setCursor(col + 1, 3);
  lcd.write(255);
  lcd.setCursor(col + 2, 3);
  lcd.write(255);
}
void print9(int col) {
  lcd.setCursor(col, 0);
  lcd.write(255);
  lcd.setCursor(col + 1, 0);
  lcd.write(255);
  lcd.setCursor(col + 2, 0);
  lcd.write(255);
  lcd.setCursor(col, 1);
  lcd.write(255);
  lcd.createChar(0, tiret_bas);
  lcd.setCursor(col + 1, 1);
  lcd.write(0);
  lcd.createChar(1, tiret_haut);
  lcd.setCursor(col + 1, 2);
  lcd.write(1);
  lcd.createChar(1, tiret_haut);
  lcd.setCursor(col, 2);
  lcd.write(1);
  lcd.setCursor(col + 2, 2);
  lcd.write(255);
  lcd.setCursor(col, 3);
  lcd.write(255);
  lcd.setCursor(col + 1, 3);
  lcd.write(255);
  lcd.setCursor(col + 2, 3);
  lcd.write(255);
  lcd.setCursor(col + 2, 1);
  lcd.write(255);
}
// DISCOVERY LOGO
byte byte00[8] = {
  0b00000,
  0b00000,
  0b00001,
  0b00001,
  0b00111,
  0b00110,
  0b00010,
  0b00111,
};
byte byte05[8] = {
  0b00000,
  0b00010,
  0b10110,
  0b11111,
  0b00000,
  0b00100,
  0b00001,
  0b10000,
};
byte byte010[8] = {
  0b00000,
  0b00000,
  0b11000,
  0b11000,
  0b01100,
  0b00110,
  0b10100,
  0b00010,
};
byte byte015[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte020[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte025[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte80[8] = {
  0b00100,
  0b00010,
  0b00110,
  0b00110,
  0b00001,
  0b00001,
  0b00000,
  0b00000,
};
byte byte85[8] = {
  0b00001,
  0b00110,
  0b10000,
  0b01000,
  0b00111,
  0b10000,
  0b00000,
  0b11111,
};
byte byte810[8] = {
  0b10011,
  0b00100,
  0b00100,
  0b01110,
  0b10000,
  0b11000,
  0b00000,
  0b10000,
};
byte byte815[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte820[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte825[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte160[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte165[8] = {
  0b01111,
  0b01111,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte1610[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte1615[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte1620[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte1625[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte240[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte245[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte2410[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte2415[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte2420[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
byte byte2425[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
};
//waiting
byte wait[8] = {
  0b11111,
  0b10001,
  0b01010,
  0b00100,
  0b01010,
  0b10001,
  0b11111,
  0b00000,
};
//smile
byte smile[] = {
  B00000,
  B00000,
  B01010,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};
// battry level
byte batt6[8] = {
  B01110,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte batt5[8] = {
  B01110,
  B01010,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};
byte batt4[8] = {
  B01110,
  B01010,
  B10001,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};
byte batt3[8] = {
  B01110,
  B01010,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111,
  B11111,
};
byte batt2[8] = {
  B01110,
  B01010,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111,
};
byte batt1[8] = {
  B01110,
  B01010,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111,
};
byte batt0[8] = {
  B01110,
  B01010,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111,
};
byte timeover1[8] = {
  B00000,
  B00111,
  B01000,
  B10000,
  B01000,
  B00100,
  B00010,
  B00001
};
//time over
byte timeover2[8] = {
  B00000,
  B11100,
  B00010,
  B00001,
  B00010,
  B00100,
  B01000,
  B10000
};
byte timeover4[8] = {
  B10000,
  B11000,
  B11100,
  B11110,
  B11111,
  B11110,
  B11100,
  B00000
};
byte timeover3[8] = {
  B00001,
  B00011,
  B00111,
  B01111,
  B11111,
  B01111,
  B00111,
  B00000
};
byte timeover5[8] = {
  B00100,
  B01110,
  B11111,
  B10101,
  B11111,
  B01010,
  B10001,
  B01010
};
//start
uint8_t heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
void discovery() {
  lcd.createChar(0, byte00);
  lcd.setCursor(8, 0);
  lcd.write(0);
  lcd.createChar(1, byte05);
  lcd.setCursor(9, 0);
  lcd.write(1);
  lcd.createChar(2, byte010);
  lcd.setCursor(10, 0);
  lcd.write(2);
  lcd.setCursor(11, 0);
  lcd.write(' ');
  lcd.setCursor(12, 0);
  lcd.write(' ');
  lcd.setCursor(13, 0);
  lcd.write(' ');
  lcd.createChar(3, byte80);
  lcd.setCursor(8, 1);
  lcd.write(3);
  lcd.createChar(4, byte85);
  lcd.setCursor(9, 1);
  lcd.write(4);
  lcd.createChar(5, byte810);
  lcd.setCursor(10, 1);
  lcd.write(5);
  lcd.setCursor(11, 1);
  lcd.write(' ');
  lcd.setCursor(12, 1);
  lcd.write(' ');
  lcd.setCursor(13, 1);
  lcd.write(' ');
  lcd.setCursor(8, 2);
  lcd.write(' ');
  lcd.createChar(6, byte165);
  lcd.setCursor(9, 2);
  lcd.write(6);
  lcd.setCursor(10, 2);
  lcd.write(' ');
  lcd.setCursor(11, 2);
  lcd.write(' ');
  lcd.setCursor(12, 2);
  lcd.write(' ');
  lcd.setCursor(13, 2);
  lcd.write(' ');
  lcd.setCursor(8, 3);
  lcd.write(' ');
  lcd.setCursor(9, 3);
  lcd.write(' ');
  lcd.setCursor(10, 3);
  lcd.write(' ');
  lcd.setCursor(11, 3);
  lcd.write(' ');
  lcd.setCursor(12, 3);
  lcd.write(' ');
  lcd.setCursor(13, 3);
  lcd.write(' ');
  lcd.setCursor(3, 3);
  String s = "DISCOVERY CLUB";
  for (int i = 0; i < s.length(); i++) {
    lcd.print(s[i]);
    delay(90);
  }
}
void waiting() {
  lcd.clear();
  lcd.backlight();
  discovery();
  delay(300);
  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print("Waiting For ");
  lcd.setCursor(3, 2);
  lcd.print("Connection");
  lcd.createChar(7, wait);
  lcd.setCursor(14, 2);
  lcd.write(7);
  while (1) {
    Usb.Task();
    Serial.println(".");
    if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
      if (PS3.getAnalogHat(LeftHatX) > 137 || PS3.getAnalogHat(LeftHatX) < 117 || PS3.getAnalogHat(LeftHatY) > 137 || PS3.getAnalogHat(LeftHatY) < 117 || PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) < 117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117) {
        Serial.print(F("\r\nLeftHatX: "));
        Serial.print(PS3.getAnalogHat(LeftHatX));
        Serial.print(F("\tLeftHatY: "));
        Serial.print(PS3.getAnalogHat(LeftHatY));
        if (PS3.PS3Connected) { // The Navigation controller only have one joystick
          Serial.print(F("\tRightHatX: "));
          Serial.print(PS3.getAnalogHat(RightHatX));
          Serial.print(F("\tRightHatY: "));
          Serial.print(PS3.getAnalogHat(RightHatY));
        }
      }
      // Analog button values can be read from almost all buttons
      if (PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
        Serial.print(F("\r\nL2: "));
        Serial.print(PS3.getAnalogButton(L2));
        if (PS3.PS3Connected) {
          Serial.print(F("\tR2: "));
          Serial.print(PS3.getAnalogButton(R2));
        }
      }
      if (PS3.getButtonClick(PS)) {
        Serial.print(F("\r\nPS"));
        PS3.disconnect();
      }
      else {
        if (PS3.getButtonClick(TRIANGLE)) {
          Serial.print(F("\r\nTriangle"));
          PS3.setRumbleOn(RumbleLow);
        }
        if (PS3.getButtonClick(CIRCLE)) {
          Serial.print(F("\r\nCircle"));
          PS3.setRumbleOn(RumbleHigh);
        }
        if (PS3.getButtonClick(CROSS)) {
          started = true;
          home();
        }
        if (PS3.getButtonClick(SQUARE))
          Serial.print(F("\r\nSquare"));

        if (PS3.getButtonClick(UP)) {
          Serial.print(F("\r\nUp"));
          if (PS3.PS3Connected) {
            PS3.setLedOff();
            PS3.setLedOn(LED4);
          }
        }
        if (PS3.getButtonClick(RIGHT)) {
          Serial.print(F("\r\nRight"));
          if (PS3.PS3Connected) {
            PS3.setLedOff();
            PS3.setLedOn(LED1);
          }
        }
        if (PS3.getButtonClick(DOWN)) {
          Serial.print(F("\r\nDown"));
          if (PS3.PS3Connected) {
            PS3.setLedOff();
            PS3.setLedOn(LED2);
          }
        }
        if (PS3.getButtonClick(LEFT)) {
          Serial.print(F("\r\nLeft"));
          if (PS3.PS3Connected) {
            PS3.setLedOff();
            PS3.setLedOn(LED3);
          }
        }
        if (PS3.getButtonClick(L1))
          Serial.print(F("\r\nL1"));
        if (PS3.getButtonClick(L3))
          Serial.print(F("\r\nL3"));
        if (PS3.getButtonClick(R1))
          Serial.print(F("\r\nR1"));
        if (PS3.getButtonClick(R3))
          Serial.print(F("\r\nR3"));
        if (PS3.getButtonClick(SELECT)) {
          Serial.print(F("\r\nSelect - "));
          PS3.printStatusString();
        }
        if (PS3.getButtonClick(START)) {
          Serial.print(F("\r\nStart"));
          printAngle = !printAngle;
        }
      }
#if 0 // Set this to 1 in order to see the angle of the controller
      if (printAngle) {
        Serial.print(F("\r\nPitch: "));
        Serial.print(PS3.getAngle(Pitch));
        Serial.print(F("\tRoll: "));
        Serial.print(PS3.getAngle(Roll));
      }
#endif
    }
#if 0 // Set this to 1 in order to enable support for the Playstation Move controller
    else if (PS3.PS3MoveConnected) {
      if (PS3.getAnalogButton(T)) {
        Serial.print(F("\r\nT: "));
        Serial.print(PS3.getAnalogButton(T));
      }
      if (PS3.getButtonClick(PS)) {
        Serial.print(F("\r\nPS"));
        PS3.disconnect();
      }
      else {
        if (PS3.getButtonClick(SELECT)) {
          Serial.print(F("\r\nSelect"));
          printTemperature = !printTemperature;
        }
        if (PS3.getButtonClick(START)) {
          Serial.print(F("\r\nStart"));
          printAngle = !printAngle;
        }
        if (PS3.getButtonClick(TRIANGLE)) {
          Serial.print(F("\r\nTriangle"));
          PS3.moveSetBulb(Red);
          digitalWrite(led, HIGH);
        }
        if (PS3.getButtonClick(CIRCLE)) {
          Serial.print(F("\r\nCircle"));
          PS3.moveSetBulb(Green);
        }
        if (PS3.getButtonClick(SQUARE)) {
          Serial.print(F("\r\nSquare"));
          PS3.moveSetBulb(Blue);
        }
        if (PS3.getButtonClick(CROSS)) {
          started = true;
          home();
          Serial.print(F("\r\nCross"));
          PS3.moveSetBulb(Yellow);
        }
        if (PS3.getButtonClick(MOVE)) {
          PS3.moveSetBulb(Off);
          Serial.print(F("\r\nMove"));
          Serial.print(F(" - "));
          PS3.printStatusString();
        }
      }
      if (printAngle) {
        Serial.print(F("\r\nPitch: "));
        Serial.print(PS3.getAngle(Pitch));
        Serial.print(F("\tRoll: "));
        Serial.print(PS3.getAngle(Roll));
      }
      else if (printTemperature) {
        Serial.print(F("\r\nTemperature: "));
        Serial.print(PS3.getTemperature());
      }
    }
#endif
  }
here :
  home();
}
void staff() {
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Special Thank");
  lcd.setCursor(2, 1);
  lcd.print("To These Heroes:");
  lcd.createChar(7, heart);
  lcd.setCursor(8, 2);
  lcd.write(7);
  lcd.setCursor(9, 2);
  lcd.write(7);
  lcd.setCursor(10, 2);
  lcd.write(7);
  lcd.setCursor(11, 2);
  lcd.write(7);
  delay(400);
  lcd.clear();
  String s[] = {"Anas Jlassi", "Maram Walhazi", "Hiba Sakka", "Wissal Naouai", "Rzouga Houidi", "Med Amin Dridi", "Med Taha Hammemi", "Mouadh Jlassi", "Wassim Toukebri", "Yassin Sakka"};
  for (int j = 0; j < sizeof(s) / 6; j++) {
    if (j % 4 == 0 && j / 4 != 0) {
      lcd.setCursor(0, 0);
      for (int i = 0; i < 80; i++) {
        lcd.print(" ");
        delay(10);
      }
      lcd.setCursor(0, 0);
    }
    lcd.setCursor(0, j % 4);
    lcd.print(j + 1);
    lcd.print(") ");
    for (int i = 0; i < s[j].length() && i < 16; i++) {
      lcd.print(s[j][i]);
      delay(10);
    }
    lcd.createChar(7, smile);
    lcd.setCursor(19, j % 4);
    lcd.write(7);
    delay(10);
  }
}
void charge(int charge) {
  if (charge > 100) charge = 100;
  if (charge < 0) charge = 0;
  lcd.setCursor(15, 0);
  lcd.print("    ");
  lcd.setCursor(15, 0);
  lcd.print(charge);
  lcd.print("%");
  if (charge > 99) lcd.createChar(7, batt6);
  else if (charge > 90) lcd.createChar(7, batt5);
  else if (charge > 70) lcd.createChar(7, batt4);
  else if (charge > 50) lcd.createChar(7, batt3);
  else if (charge > 30) lcd.createChar(7, batt2);
  else if (charge > 10) lcd.createChar(7, batt1);
  else lcd.createChar(7, batt0);
  lcd.setCursor(19, 0);
  lcd.write(7);
  Serial.println("charge");
}
void score(int c) {
  score_total += c;
  if (score_total < 0) {
    score_total = 0;
  }
  lcd.clear();
  printnumber_2(score_total);
  charge(50);
  lcd.setCursor(15, 2);
  lcd.print("TIME:");
}
void printnumber_2(int x) {
  int y = x / 100 ;
  Serial.println(y);
  switch (y) {
    case 0: print0(0); break;
    case 1: print1(0); break;
    case 2: print2(0); break;
    case 3: print3(0); break;
    case 4: print4(0); break;
    case 5: print5(0); break;
    case 6: print6(0); break;
    case 7: print7(0); break;
    case 8: print8(0); break;
    case 9: print9(0); break;
  }
  int d = (x / 10) % 10;
  Serial.println(d);
  switch (d) {
    case 0: print0(4); break;
    case 1: print1(4); break;
    case 2: print2(4); break;
    case 3: print3(4); break;
    case 4: print4(4); break;
    case 5: print5(4); break;
    case 6: print6(4); break;
    case 7: print7(4); break;
    case 8: print8(4); break;
    case 9: print9(4); break;
  }
  int a = x % 10;
  Serial.println(a);
  switch (a) {
    case 0: print0(8); break;
    case 1: print1(8); break;
    case 2: print2(8); break;
    case 3: print3(8); break;
    case 4: print4(8); break;
    case 5: print5(8); break;
    case 6: print6(8); break;
    case 7: print7(8); break;
    case 8: print8(8); break;
    case 9: print9(8); break;
  }
}
void printnumber(int x) {
  int y = x / 100 ;
  Serial.println(y);
  switch (y) {
    case 0: print0(4); break;
    case 1: print1(4); break;
    case 2: print2(4); break;
    case 3: print3(4); break;
    case 4: print4(4); break;
    case 5: print5(4); break;
    case 6: print6(4); break;
    case 7: print7(4); break;
    case 8: print8(4); break;
    case 9: print9(4); break;
  }
  int d = (x / 10) % 10;
  Serial.println(d);
  switch (d) {
    case 0: print0(8); break;
    case 1: print1(8); break;
    case 2: print2(8); break;
    case 3: print3(8); break;
    case 4: print4(8); break;
    case 5: print5(8); break;
    case 6: print6(8); break;
    case 7: print7(8); break;
    case 8: print8(8); break;
    case 9: print9(8); break;
  }
  int a = x % 10;
  Serial.println(a);
  switch (a) {
    case 0: print0(12); break;
    case 1: print1(12); break;
    case 2: print2(12); break;
    case 3: print3(12); break;
    case 4: print4(12); break;
    case 5: print5(12); break;
    case 6: print6(12); break;
    case 7: print7(12); break;
    case 8: print8(12); break;
    case 9: print9(12); break;
  }
}
void time() {
  if (started == true && remaining_time > 0) {
    remaining_time--;
    Serial.println(remaining_time);
    if (remaining_time == 0) {
      game_over = true;
    }
  }
}

/////////////////////////////// brassssssssssss///////////

void countEncoderRA() {
  encoderRA_set = digitalRead(encoderRA);
  encoderRPos += (encoderRA_set == encoderRB_set) ? -1 : +1;
  dist += (encoderRA_set == encoderRB_set) ? -distT : +distT;
}

void countEncoderRB() {
  encoderRB_set = digitalRead(encoderRB);
  encoderRPos += (encoderRA_set != encoderRB_set) ? -1 : +1;
  dist += (encoderRA_set != encoderRB_set) ? -distT : +distT;
}
void MoveDown(int a) {
  analogWrite(motorDown, a);
  analogWrite(motorUp, 0);
}
void MoveUp(int b) {
  analogWrite(motorUp, b);
  analogWrite(motorDown, 0);
}

void STOP() {
  analogWrite(motorUp, 0);
  analogWrite(motorDown, 0);
}

void initial() {
  currentE = 0;
  sommeE = 0;
  diffE = 0;
  prevE = 0;
  initalError = 0;
  cmd = 0;
}

void set_consigne(float a) {
  consigne = a;
}

void Move_2(float d) {
  int counter = 0;
  float intialdist = dist;
  if (d > 200)
    Move(200);
  else {
    initial();
    set_consigne(d);
    initalError = consigne - dist;
    currentE = consigne - dist;
    while (abs(currentE) > 0.5) {
      // Serial.println(dist-intialdist);
      counter++;
      if (counter > 100) {
        counter = 0;
        Serial.print("eeeeeeeeeeeeeeeeeeeeeeeee ");
        Serial.println(dist - intialdist);
        intialdist = dist;
      }
      currentE = consigne - dist;
      sommeE = sommeE + currentE;
      diffE = currentE - prevE;
      prevE = currentE;
      cmd = int(Kp * currentE + Ki * sommeE + Kd * diffE);
      cmd = abs(cmd);
      if (cmd > 255)
        cmd = 255;
      if (cmd < 150)
        cmd = 150;
      if (currentE > 0)
        MoveDown(cmd);
      else if (currentE < 0)
        MoveUp(cmd);
      //  Serial.print("  position    ");
      //   Serial.print(encoderRPos);
      //Serial.print("  distance   ");
      //   Serial.println(dist);
    }
    STOP();
    initial();
  }
}

void Move(float d) {
  int counter = 0;
  float intialdist = dist;
  float dist0 = dist;
  if (d > 200)
    Move(200);
  else {
    initial();
    set_consigne(d);
    initalError = consigne - dist;
    currentE = consigne - dist;
    while (abs(currentE) > 0.5) {
      // Serial.println(dist-intialdist);
      counter++;
      if (counter > 100) {
        counter = 0;

        Serial.print("eeeeeeeeeeeeeeeeeeeeeeeee ");
        Serial.println(dist - intialdist);
        Serial.println(dist);
        Serial.println(intialdist);
        Serial.println("3rrrrrrrrrrrrrrrrrrrrrr");
        if (abs(dist - intialdist) < 1.5 && abs(dist0 - dist) > 20  && dist > 20) {


          intialdist = dist;

          break;
        }
        intialdist = dist;
      }
      currentE = consigne - dist;
      sommeE = sommeE + currentE;
      diffE = currentE - prevE;
      prevE = currentE;
      cmd = int(Kp * currentE + Ki * sommeE + Kd * diffE);
      cmd = abs(cmd);
      if (cmd > 255)
        cmd = 255;
      if (cmd < 150)
        cmd = 150;
      if (currentE > 0)
        MoveDown(cmd);
      else if (currentE < 0)
        MoveUp(cmd);
      //  Serial.print("  position    ");
      //   Serial.print(encoderRPos);
      //Serial.print("  distance   ");
      //   Serial.println(dist);
    }
    STOP();
    initial();
  }
  Serial.print("fin  moveeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee ");

}
void initialRepere() {
  initial() ;
  encoderRPos = 0;
  lastEncoderRPos = 0;
  dist = 0;

}
void GlissZero() {

  val1 = digitalRead(FinCourse);
  while (val1 == 0) {
    MoveUp(255);
    val1 = digitalRead(FinCourse);
    Serial.println("gggggggggggggggggggggggggggggggggggggggggggggggg");
  }
  STOP() ;
  initialRepere();
}
int servo1Min = 0;
int servo1Max = 180;
int servo2Min = 30;
int servo2Max = 140;
int pompePin = 22;

void initBras() {
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo1.write(90);
  servo2.write(servo2Min);
  pompeOff();
}

void lbarra() {
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo1.write(servo1Min);
  servo2.write(servo2Min);
  //delay(1000);
  //servo1.detach();
  //servo2.detach();

}

void ld5el() {
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo1.write(servo1Max);
  servo2.write(servo2Max);
  
  //delay(1000);
  //servo1.detach();
  //servo2.detach();
}

void pompeOn() {
  digitalWrite(pompePin, HIGH);
}

void pompeOff() {
  digitalWrite(pompePin, LOW);
}
void int_pos() {
  while (digitalRead(FinCourse)) {
    MoveUp(200);
  }
}
int brasPos = 25;
void  evacuation_n2() {
  //bala.attach(bala_pin);
  Move_2(40);
pompeOn();
  ld5el();
  delay(1000);
  
  Move(500);
  //delay(1000);

  //delay(2000);

  //delay(1000);
  //Serial.println("fgggggggggggggggggggggggggggggg");
  GlissZero();
  lbarra();
  //delay(2000);
  //delay(500);

//bala.detach();
}
void  evacuation_n1() {
  //bala.attach(bala_pin);
  Move_2(40);
  pompeOn();
  ld5el();
  delay(1000);
  
  Move(500);
 
  Serial.println("fgggggggggggggggggggggggggggggg");
  Move_2(40);
  lbarra();
  delay(1000);
  Move_2(90);
//bala.detach();
}
/*
  delay(2000);
  pompeOff();
  delay(1000);
  ld5el();
  Move(40);
  delay(1000);
  ld5el();
  delay(3000);
  pompeOn();
  delay(1000);
  GlissZero();
  lbarra();
  delay(3000);
  pompeOff();
  delay(1000);
  ld5el();
  Move(60);
  delay(1000);
  ld5el();
  delay(3000);
  pompeOn();
  delay(1000);
  GlissZero();
  lbarra();
  delay(3000);
  pompeOff(); 
  delay(1000);


  }*/
//////////////////////////////////////////////

////////////////////////// PS3 ///////////////
void control() {
  while (1) {
    //test();
    Usb.Task();
    if (PS3.getAnalogHat(RightHatY) <117) {
      res_down();
    }

    if(PS3.getAnalogHat(LeftHatY)<50 && PS3.getAnalogHat(LeftHatX)<50){
      Serial.println("for right");
        forwardRight();
      }
      else if(PS3.getAnalogHat(LeftHatY)<50 && PS3.getAnalogHat(LeftHatX)>200){
        Serial.println("for left");
        forwardLeft();
      }
      
     else{
      if(PS3.getAnalogHat(LeftHatY)>200){
        Serial.println("for ");
        forward();
      }else 
      if(PS3.getAnalogHat(LeftHatY)<50){
      backward();
      }
      else 
      if(PS3.getAnalogHat(LeftHatX)>200){
        Serial.println("left");
      left();
      }
      else 
      if(PS3.getAnalogHat(LeftHatX)<50){
        Serial.println("right");
      right();
      }else {
        Stop();
        }
     }
      

      
    if (PS3.getButtonClick(TRIANGLE)) {
      evacuation_n2();

    }

    if (PS3.getButtonClick(CIRCLE)) {
      r_pince_close();
      l_pince_close();

    }

    if (PS3.getButtonClick(CROSS)) {
      initBras();
      r_pince_open();
      l_pince_open();


    }

    if (PS3.getButtonClick(SQUARE)) {
      //Serial.println("fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
      pompeOff();
      servo1.detach();
      servo2.detach();
    }


    if (PS3.getButtonClick(UP)) {
      score(10);

    }
    if (PS3.getButtonClick(RIGHT)) {
      score(1);

    }
    if (PS3.getButtonClick(DOWN)) {
      score(-10);

    }
    if (PS3.getButtonClick(LEFT)) {
      score(-1);

    }

    if (PS3.getButtonClick(R1)) {
      Serial.println("uppppppppppppppppppppppppppppppppppppppppppppppppp");
      bala_up();

    }
    if (PS3.getButtonClick(R2)) {
      Serial.println("downnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn");
      initBras();
      bala_down();

    }

    if (PS3.getButtonClick(R3)) {

      evacuation_n1();
    }


    if (PS3.getButtonClick(L1)) {
      
      int x=res.read()-5;
      if (x<0){x=0;}
      res.write(x);
      
      
    }

    if (PS3.getButtonClick(L2)) {
      res_up();
      delay(700);
      res_down();
    }
    if (PS3.getButtonClick(L3)) {
      initBras();
    

    }

    if (PS3.getButtonClick(START)) {
      vit+=10;
        if(vit >255) vit =255;
    }
    if (PS3.getButtonClick(SELECT)) {
      
     vit-=10;
        if(vit <0) vit =0;
    }
  }
}
void r_pince_open() {
  /*r_pincepos=r_pincepos+50;
    if(r_pincepos > 160 ){
     r_pincepos=160;
     }*/
  r_pince.attach(r_pince_pin);
  r_pince.write(160);
  r_pincepos = 150;
  //delay(500);
  //r_pince.detach();
}
void r_pince_close() {

  r_pincepos = r_pincepos - 15;
  if (r_pincepos <  0) {
    r_pincepos = 0;

  }
  r_pince.attach(r_pince_pin);
  r_pince.write(r_pincepos);

}
void l_pince_open() {
  /*l_pincepos=l_pincepos-50;
    if(l_pincepos < 0){
     l_pincepos=0;
     }*/
  l_pince.attach(l_pince_pin);
  l_pince.write(0);
  l_pincepos = 0;
  delay(500);
  r_pince.detach();
}
void l_pince_close() {
  l_pincepos = l_pincepos + 15;
  if (l_pincepos > 150) {
    l_pincepos = 150;
  }
  l_pince.attach(l_pince_pin);
  l_pince.write(l_pincepos);
}
void bala_up() {
  bala.attach(bala_pin);
  bala.write(135);
  //delay(1000);
  //bala.detach();
}
void bala_down() {
  bala.attach(bala_pin);
  bala.write(89);
  //delay(1000);  
  //bala.detach();
}
void res_up() {
  res.attach(res_pin);
  res.write(0);

}
void res_down() {

  res.attach(res_pin);
  res.write(110);

}
void hakeka_up() {
  hakekapos = hakekapos - 10;
  if (hakekapos < 30 ) {
    hakekapos = 30;
  }
  hakeka.attach(hakeka_pin);
  hakeka.write(hakekapos);



}
void hakeka_down() {
  hakekapos = 100;
  hakeka.attach(hakeka_pin);
  hakeka.write(100);

}
void test() {
  pinMode(A11,INPUT);
  int x = analogRead(A11);
  Serial.println(analogRead(A11));
  if (x > 700 && x < 960) {
    digitalWrite(led_bleu_pin, HIGH);
    digitalWrite(led_red_pin, LOW);
    digitalWrite(led_orange_pin, LOW);
  }
  else if (x > 400 && x < 960) {
    digitalWrite(led_orange_pin, HIGH);
    digitalWrite(led_bleu_pin, LOW);
    digitalWrite(led_red_pin, LOW);
  }
  else if (x > 100 && x < 960) {
    digitalWrite(led_red_pin, HIGH);
    digitalWrite(led_bleu_pin, LOW);
    digitalWrite(led_orange_pin, LOW);
  }
  else {
    digitalWrite(led_red_pin, HIGH);
    digitalWrite(led_bleu_pin, HIGH);
    digitalWrite(led_orange_pin, HIGH);
  }
}

// les fonction de mouvement de robot

void forwardRight() {
  analogWrite(md1, LOW);
  analogWrite(md2, vit-50);
  analogWrite(mg1, LOW );
  analogWrite(mg2, vit);
}
void forwardLeft() {
  analogWrite(md1, LOW);
  analogWrite(md2, vit);
  analogWrite(mg1, LOW);
  analogWrite(mg2, vit-50);
}

void forward() {
  analogWrite(md1, vit);
  analogWrite(md2, LOW);
  analogWrite(mg1, vit );
  analogWrite(mg2, LOW);
}
void backward() {
  analogWrite(md1, LOW);
  analogWrite(md2, vit);
  analogWrite(mg1, LOW);
  analogWrite(mg2, vit);
}
void right() {
  analogWrite(md1, vit);
  analogWrite(md2, LOW);
  analogWrite(mg1, LOW);
  analogWrite(mg2, vit);
}
void left() {
  analogWrite(md1, LOW);
  analogWrite(md2, vit);
  analogWrite(mg1, vit);
  analogWrite(mg2, LOW);
}
void Stop() {
  analogWrite(md1, LOW);
  analogWrite(md2, LOW);
  analogWrite(mg1, LOW);
  analogWrite(mg2, LOW);
}

void home() {
  bool s = false;
  GlissZero();
  initBras();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(7, 0);
  lcd.print("Score:");
  lcd.setCursor(16, 2);
  lcd.print("Time:");
  score(50);

  control();

}


// servos
void gameover() {
  bool s = false;
  lcd.clear();
  lcd.backlight();

  lcd.createChar(1, timeover1);
  lcd.setCursor(5, 1);
  lcd.write(1) ;
  lcd.setCursor(13, 1);
  lcd.write(1);

  lcd.createChar(2, timeover2);
  lcd.setCursor(6, 1);
  lcd.write(2);
  lcd.setCursor(14, 1);
  lcd.write(2);

  lcd.createChar(3, timeover3);
  lcd.setCursor(5, 2);
  lcd.write(3);
  lcd.setCursor(13, 2);
  lcd.write(3);

  lcd.createChar(4, timeover4);
  lcd.setCursor(6, 2);
  lcd.write(4);
  lcd.setCursor(14, 2);
  lcd.write(4);

  lcd.setCursor(8, 1);
  lcd.print("GAME");
  lcd.setCursor(8, 2);
  lcd.print("OVER");

  lcd.createChar(5, timeover5);
  lcd.setCursor(3, 1);
  lcd.write(5);
  lcd.setCursor(3, 2);
  lcd.write(5);
  lcd.setCursor(16, 1);
  lcd.write(5);
  lcd.setCursor(16, 2);
  lcd.write(5);
  delay(500);
  lcd.clear();
  printnumber(score_total);


  control();
}
void setup()
{
  pinMode(led_red_pin, OUTPUT);
  pinMode(led_bleu_pin, OUTPUT);
  pinMode(led_orange_pin, OUTPUT);

  pinMode(md1, OUTPUT);
  pinMode(md2, OUTPUT);
  pinMode(mg1, OUTPUT);
  pinMode(mg2, OUTPUT);
  pinMode(test_pin, INPUT);

  pinMode(A15, INPUT);
  pinMode(30, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(34, OUTPUT);

  digitalWrite(encoderRA, HIGH);
  digitalWrite(encoderRB, HIGH);
  attachInterrupt(digitalPinToInterrupt(18), countEncoderRA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(19), countEncoderRB, CHANGE);
  pinMode( pompePin, OUTPUT);
  digitalWrite(pompePin, LOW);
  pinMode(FinCourse, INPUT_PULLUP);
  pinMode(encoderRA, INPUT);
  pinMode(encoderRB, INPUT);

  pinMode(motorDown, OUTPUT);
  pinMode(motorUp, OUTPUT);
  Serial.begin(115200);
  res_down();
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
  
  //Serial.println("Starting...");


  lcd.init();
  lcd.backlight();
  //staff();
  waiting();
  
}
void loop() {

}
