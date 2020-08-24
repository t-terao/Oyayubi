#include <Wire.h>

//MKPU-6050のアドレスとレジスタの設定
#define MPU6050_WHO_AM_I 0x75 //Read Only
#define MPU6050_PWR_MGMT_1 0x6B //Read and Write
#define MPU_ADDRESS 0x68
#define Arduino 8 //Slave ID
int rotatey=90;
int rotatez=90;
char c;
long y;
byte by = 0;
byte bz = 0;
byte az = 0;
int iy = 0;
int iz = 0;
void setup() {
  Serial.begin(9600);
  //I2Cスタート
  Wire.begin(21,22); //SDA:21 SCL:22

  //初回読み出し
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(MPU6050_WHO_AM_I);
  Wire.write(0x00);
  Wire.endTransmission();

  //動作モードの読み出し
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(MPU6050_PWR_MGMT_1);
  Wire.write(0x00);
  Wire.endTransmission();
}
void getXYZ(){
  y=0;
  for(int i=0;i<50;i++){
    y+=analogRead(34);
  }
  y=y/500;
}

double getrotateY(){
  double rotate;
  rotate = (y-105)/0.9;
  return rotate;
}
void loop() {
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(0x68, 14, true);
  while (Wire.available() < 14);
  int16_t axRaw, ayRaw, azRaw, gxRaw, gyRaw, gzRaw, Temperature;

  axRaw = Wire.read() << 8 | Wire.read();
  ayRaw = Wire.read() << 8 | Wire.read();
  azRaw = Wire.read() << 8 | Wire.read();
  Temperature = Wire.read() << 8 | Wire.read();
  gxRaw = Wire.read() << 8 | Wire.read();
  gyRaw = Wire.read() << 8 | Wire.read();
  gzRaw = Wire.read() << 8 | Wire.read();

  // 加速度値を分解能で割って加速度(G)に変換する
  float acc_x = axRaw / 16384.0;  //FS_SEL_0 16,384 LSB / g
  float acc_y = ayRaw / 16384.0;
  float acc_z = azRaw / 16384.0;

  // 角速度値を分解能で割って角速度(degrees per sec)に変換する
  float gyro_x = gxRaw / 131.0;//FS_SEL_0 131 LSB / (°/s)
  float gyro_y = gyRaw / 131.0+3.5;
  float gyro_z = gzRaw / 131.0;
  if(gyro_y>5 || gyro_y<-5||gyro_z>5||gyro_z<-5){
    getXYZ();
    iy = (int)getrotateY();
    Serial.println(iy);
    iz = (int)gyro_z;
    Wire.beginTransmission(Arduino);
    by=byte(iy);
    Wire.write(by);
    if(iz<0){
      iz=-iz;
      az=0;
      bz = byte(iz);
      Wire.write(bz);
      Wire.write(az);
    }else if(iz>0){
      az = 1;
      bz = byte(iz);
      Wire.write(bz);
      Wire.write(az);
    }
    Wire.endTransmission();
  }
  if(gyro_z>5 || gyro_z<-5){

    Wire.beginTransmission(Arduino);
    Wire.write("z");

    Wire.endTransmission();
  } 
  //Serial.print(acc_x);  Serial.print(",");
 // Serial.print(acc_y);  Serial.print(",");
  //Serial.print(acc_z);  Serial.print(",");
  //Serial.print(gyro_x); Serial.print(",");Serial.println(rotate);
  Serial.print("y:");Serial.print(gyro_y); 
  Serial.print("z:");Serial.println(gyro_z);
  delay(200);
}
