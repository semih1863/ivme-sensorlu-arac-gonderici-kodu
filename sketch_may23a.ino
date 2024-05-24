#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <MPU6050.h>

RF24 radio(9, 10); // CE, CSN pinleri
const byte address[6] = "00001";
MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 bağlantısı başarısız!");
    while (1);
  }

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}

void loop() {
  float x = mpu.getAccelerationX() / 16384.0;
  float y = mpu.getAccelerationY() / 16384.0;
  float z = mpu.getAccelerationZ() / 16384.0;

  // Verileri seri monitöre yazdır
  Serial.print("X: "); Serial.print(x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(z); Serial.println("");

  // Verileri alıcıya gönder
  radio.write(&x, sizeof(x));
  radio.write(&y, sizeof(y));
  radio.write(&z, sizeof(z));

  delay(100); // Veri gönderme aralığını belirle
}
