#include <Wire.h>
#include <Servo.h>
Servo zservo;
Servo yservo;
int rotatey = 90;
int rotatez = 90;
void setup() {
  // put your setup code here, to run once:
  Wire.begin(8);
  Serial.begin(9600);
  zservo.attach(13);
  zservo.write(rotatez);
  yservo.attach(12);
  yservo.write(rotatey);
  Wire.onReceive(myReadLine);
}

void loop() {
}

void myReadLine(int a) {
  int ay = 0;
  int by = 0;
  int az = 0;
  int bz = 0;
  if (Wire.available()) by = (int)Wire.read();
  if (Wire.available()) bz = (int)Wire.read();
  if (Wire.available()) az = (int)Wire.read();
  if (az == 0) {
    bz = -bz;
  }
  rotatey=by;
  rotatez = rotatez + bz * 0.5;
  if (rotatez < 0)rotatez = 0;
  if (rotatez > 180)rotatez = 180;
  zservo.write(rotatez);
  Serial.print("z:");Serial.println(rotatez);
  yservo.write(rotatey);
  Serial.print("y:");Serial.println(rotatey);

}
