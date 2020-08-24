#include <ESP32Servo.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;
Servo zservo;
Servo yservo;
int zservopin = 32;
int yservopin = 33;
int minUs = 500;
int maxUs = 2400;
int ay = 0;
int iy = 90;
int az = 0;
int iz = 90;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  zservo.setPeriodHertz(50);
  yservo.setPeriodHertz(50);
  zservo.attach(zservopin, minUs, maxUs);
  yservo.attach(yservopin, minUs, maxUs);
  zservo.write(iz);
  yservo.write(iy);
}

void loop() {
  if (SerialBT.available()) iy = (int)SerialBT.read();
  //if (SerialBT.available()) ay = (int)Wire.read();
  if (SerialBT.available()) iz = (int)SerialBT.read();
    zservo.write(iz);
    Serial.println(iz);
    yservo.write(iy);
    Serial.println(iy);
}
