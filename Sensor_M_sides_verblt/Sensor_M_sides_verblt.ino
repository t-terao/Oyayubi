#include <Wire.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

//MKPU-6050のアドレスとレジスタの設定
#define MPU6050_WHO_AM_I 0x75 //Read Only
#define MPU6050_PWR_MGMT_1 0x6B //Read and Write
#define MPU_ADDRESS 0x68
#define ysensor 34
#define y_min 105
#define y_range 0.9
String MACadd = "24:6F:28:79:CF:FA";
uint8_t address[6]  = {0x24, 0x6F, 0x28, 0x79, 0xCF, 0xFA};
//uint8_t address[6]  = {0x00, 0x1D, 0xA5, 0x02, 0xC3, 0x22};
String name = "OBDII";
char *pin = "1234"; //<- standard pin would be provided by default
bool connected;
char c;
long y;
int rotatey = 90;
int rotatez = 90;
int iy_b = 90;
byte by = 0;
byte bz = 0;
int az = 0;
int iy = 0;
int iz = 0;
void setup() {
  Serial.begin(9600);
  //I2Cスタート
  Wire.begin(21, 22); //SDA:21 SCL:22
  SerialBT.begin("ESP32test", true);
  Serial.println("The device started in master mode, make sure remote BT device is on!");
  connected = SerialBT.connect(address);

  if (connected) {
    Serial.println("Connected Succesfully!");
  } else {
    while (!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app.");
    }
  }
  // disconnect() may take upto 10 secs max
  if (SerialBT.disconnect()) {
    Serial.println("Disconnected Succesfully!");
  }
  // this would reconnect to the name(will use address, if resolved) or address used with connect(name/address).
  SerialBT.connect();
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
  SerialBT.write(rotatey);
  SerialBT.write(rotatez);
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


  // 角速度値を分解能で割って角速度(degrees per sec)に変換する
  //FS_SEL_0 131 LSB / (°/s)
  float gyro_y = gyRaw / 131.0+3.5;
  float gyro_z = gzRaw / 131.0;
  if((gyro_y > 40 || gyro_y<-40)&&(gyro_z>40||gyro_z<-40)){
    iy = (int)gyro_y;
    iz = (int)gyro_z;
    rotatey = rotatey + iy*0.2;
    rotatez = rotatez + iz*0.2;
    if(rotatey<20)rotatey=20;
    if(rotatey>160)rotatey=160;
    if(rotatez<20)rotatez=20;
    if(rotatez>160)rotatez=160;
    SerialBT.write(rotatey);
    SerialBT.write(rotatez);
    Serial.print("rotatey:");Serial.println(rotatey);
    Serial.print("rotatez:");Serial.println(rotatez);
  }
  else if (gyro_y > 40 || gyro_y<-40) {
    iy = (int)gyro_y;
    rotatey = rotatey + iy*0.2;
    if(rotatey<20)rotatey=20;
    if(rotatey>160)rotatey=160;
    SerialBT.write(rotatey);
    SerialBT.write(rotatez);
    Serial.print("rotatey:");Serial.println(rotatey);
  }
  else if(gyro_z >40||gyro_z<-40){
    iz=(int)gyro_z;
    rotatez = rotatez + iz*0.2;
    if(rotatez<20)rotatez=20;
    if(rotatez>160)rotatez=160;
    SerialBT.write(rotatey);
    SerialBT.write(rotatez);
    Serial.print("rotatez:");Serial.println(rotatez);
  }

  //Serial.print(acc_x);  Serial.print(",");
  // Serial.print(acc_y);  Serial.print(",");
  //Serial.print(acc_z);  Serial.print(",");
  //Serial.print(gyro_x); Serial.print(",");Serial.println(rotate);
  Serial.print("y:");Serial.print(gyro_y);
  Serial.print("z:"); Serial.println(gyro_z);
  delay(300);
}
