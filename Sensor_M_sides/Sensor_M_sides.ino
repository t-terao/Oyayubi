#include <Wire.h>

//MKPU-6050のアドレスとレジスタの設定
#define MPU6050_WHO_AM_I 0x75 //Read Only
#define MPU6050_PWR_MGMT_1 0x6B //Read and Write
#define MPU_ADDRESS 0x68
#define Arduino 8 //Slave ID
int rotatey=90;
int rotatez=90;
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
  float gyro_y = gyRaw / 131.0;
  float gyro_z = gzRaw / 131.0;
  if(gyro_y>5 || gyro_y<-5){
    int i = (int)gyro_y;
    String c = String(i);
    Serial.print(i);Serial.print(c);
    Wire.beginTransmission(Arduino);
    Wire.write("y");
    Wire.write(c);
    Wire.endTransmission();
  }
  if(gyro_z>5 || gyro_z<-5){
    int i = (int)gyro_z;
    Strint c = String(i);
    Serial.print(i);Serial.print(c);
    Wire.beginTransmission(Arduino);
    Wire.write("z");
    Wire.write(c);
    Wire.endTransmission();
  } 
  //Serial.print(acc_x);  Serial.print(",");
 // Serial.print(acc_y);  Serial.print(",");
  //Serial.print(acc_z);  Serial.print(",");
  //Serial.print(gyro_x); Serial.print(",");Serial.println(rotate);
  Serial.print("y:");Serial.print(gyro_y); Serial.print(",");Serial.println(rotatey);
  Serial.print("z:");Serial.print(gyro_z); Serial.println(rotatez);
  delay(100);
}
