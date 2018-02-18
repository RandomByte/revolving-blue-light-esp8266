
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C

const int outPinMotor = D1;
const int outPinLed = D2;
const int pinScl = D3;
const int pinSda = D4;


void setup() {
  pinMode(outPinMotor, OUTPUT);
  pinMode(outPinLed, OUTPUT);

  Serial.begin(9600);
  Serial.println(F("BME280 test"));
  delay(10);
  Wire.begin(pinSda, pinScl);
  if (!bme.begin()) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
  }
  
  Serial.println("-- Default Test --");
  Serial.println();
}

void loop() {
  digitalWrite(outPinMotor, HIGH);
  digitalWrite(outPinLed, HIGH);
  delay(2000);
  printValues();
  delay(2000);
  digitalWrite(outPinMotor, LOW);
  digitalWrite(outPinLed, LOW);
  delay(6000);
}



void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");
  
    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}
