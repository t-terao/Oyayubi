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
    int d=0;
    if(Wire.available()) c = (char)Wire.read();
    if(Wire.available()) d = (int)Wire.read();
    Serial.println(c);
    Serial.println(d);
    if(c=='z'){
      rotatez += d*0.001;
      if(rotatez<0)rotatez=0;
      if(rotatez>180)rotatez=180;
      zservo.write(rotatez);
      Serial.println(rotatez);
    }
    if(c=='y'){
      rotatey += d*0.001;
      if(rotatey<0)rotatey=0;
      if(rotatey>180)rotatez=180;
      yservo.write(rotatey);
      Serial.println(rotatey);
    }
}
