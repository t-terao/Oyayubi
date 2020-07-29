//加速度センサの値取得プログラム

long x,y,z;

void setup() {
  // シリアルモニターの初期化
  Serial.begin(9600);
}
void getXYZ(){
  x=y=z=0;
  for(int i=0;i<50;i++){
    x+=analogRead(15);
    y+=analogRead(2);
    z+=analogRead(0);
  }
  x=x/500;
  y=y/500;
  z=z/500;
}

double getrotateX(){
  double rotate;
  rotate = (x-111)/0.911 - 90;
  return rotate;
}
double getrotateY(){
  double rotate;
  rotate = (y-111)/0.916 -90;
  return rotate;
}
void loop() {
  getXYZ();
  Serial.print("x:");
  Serial.print(x);
  Serial.print("y:");
  Serial.print(y);
  Serial.print("Z:");
  Serial.print(z);
  Serial.println("");
  Serial.print("x:");
  Serial.print(getrotateX());
  Serial.print("° ");
  Serial.print("y:");
  Serial.print(getrotateY());
  Serial.print("° ");
  Serial.println("");
  delay(500);
}
