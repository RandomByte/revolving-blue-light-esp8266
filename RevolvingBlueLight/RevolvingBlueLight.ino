
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C

const int outPinMotor = D1;
const int outPinLed = D2;
const int pinScl = D3;
const int pinSda = D4;

const int comOn = 73; // "I"
const int comOff = 79; // "O"
const int comFlash = 70; // "F"
const int comTemp = 84; // "T"
const int comSyn = 83; // "T"

const unsigned long maxRunTime = 120000; // 2 min
unsigned long startTime = 0;
int incomingByte = 0;

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
  Serial.println("A");
}

void loop() {
  if(Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
        
      switch(incomingByte) {
        case comOn:
          on();
          Serial.println("A");
          delay(300); // rate limiting
          break;
        case comOff:
          off();
          Serial.println("A");
          delay(300); // rate limiting
          break;
        case comFlash:
          on();
          delay(1000);
          off();
          Serial.println("A");
          delay(300); // rate limiting
          break;
        case comTemp:
          sendTemp();
          Serial.println("A");
          delay(300); // rate limiting
          break;
        case comSyn:
          Serial.println("A");
          break;
    }
  }
  if (startTime != 0 && (millis() - startTime) > maxRunTime) {
    off();
  }
  delay(200);
}

void on() {
  startTime = millis();
  digitalWrite(outPinMotor, HIGH);
  digitalWrite(outPinLed, HIGH);
}

void off() {
  if (millis() - startTime < 500) { // rate limiting
    delay(500);
  }
  startTime = 0;
  digitalWrite(outPinMotor, LOW);
  digitalWrite(outPinLed, LOW);
}

void sendTemp() {
  Serial.println(bme.readTemperature());
  Serial.println(bme.readHumidity());
}

