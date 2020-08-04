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
  zservo.write(90);
  yservo.attach(12);
  yservo.write(90);
  Wire.onReceive(myReadLine);
}

void loop() {
}

void myReadLine(int a){
    char c;
    int e = 0;
    int d=0;
    if(Wire.available()) c = (char)Wire.read();
    if(Wire.available()) d = (int)Wire.read();
    if(Wire.available()) e = (int)Wire.read();
    if(e==0){
      d = -d;
    }
    Serial.println(c);
    Serial.println(d);
    if(c=='z'){
      rotatez = rotatez + (d*0.002);
      if(rotatez<0)rotatez=0;
      if(rotatez>180)rotatez=180;
      zservo.write(rotatez);
      Serial.println(rotatez);
    }
    if(c=='y'){
      rotatey =rotatey + (d*0.0002);
      if(rotatey<0)rotatey=0;
      if(rotatey>180)rotatez=180;
      yservo.write(rotatey);
      Serial.println(rotatey);
    }
}
