#include <Wire.h>
#include <Adafruit_MLX90614.h>

#define PRE_CORR_FACTOR 0.0
#define CORRECTION_FACTOR  -0.75

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

char payload[10];
char bodyTemp[6], ambTemp[6], objTemp[6];

float calculateForeheadToBody(float y) {
  //https://docs.google.com/spreadsheets/d/1EtJas-raX3Qd3OiARKWVPCCqhRaQXOXToTx4brYeH8Q/edit?usp=sharing
  //based on https://www.omronhealthcare-ap.com/Content/uploads/products/68363e5e293440bf95f724dceb4fa860.pdf
  //page 22

  float x = y + PRE_CORR_FACTOR;
  return (x * x) * 0.1944810311 - (x * 12.80081016) + 247.1541305 + CORRECTION_FACTOR;
}

void setup() {
  pinMode(A2,INPUT_PULLUP);
  Serial.begin(9600);
  mlx.begin();
}

void loop() {
  float readObj = mlx.readObjectTempC();
  
  dtostrf(calculateForeheadToBody(readObj), 4, 2, bodyTemp);
  dtostrf(mlx.readAmbientTempC(), 4, 2, ambTemp);
  dtostrf(readObj, 4, 2, objTemp);
  //0=object detected, 1=object undetected, range object 30cm 
  sprintf(payload,"%s:%s:%s:%d\r\n",bodyTemp,objTemp,ambTemp,digitalRead(A2));
  Serial.print(payload);
  delay(200);
}
