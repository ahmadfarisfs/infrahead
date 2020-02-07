#include <Wire.h>
#include <Adafruit_MLX90614.h>

#define PRE_CORR_FACTOR 0.0
#define CORRECTION_FACTOR  -0.75

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float calculateForeheadToBody(float y) {
  //https://docs.google.com/spreadsheets/d/1EtJas-raX3Qd3OiARKWVPCCqhRaQXOXToTx4brYeH8Q/edit?usp=sharing
  //based on https://www.omronhealthcare-ap.com/Content/uploads/products/68363e5e293440bf95f724dceb4fa860.pdf
  //page 22

  float x = y + PRE_CORR_FACTOR;
  return (x * x) * 0.1944810311 - (x * 12.80081016) + 247.1541305 + CORRECTION_FACTOR;
}

void setup() {
  Serial.begin(9600);
  Serial.println("MLX90614 Body Temperature Measurement");
  mlx.begin();
}

void loop() {
  float objTemp = mlx.readObjectTempC();
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC());
  Serial.print("Body = "); Serial.print(calculateForeheadToBody(objTemp));
  Serial.print("*C\tObject = "); Serial.print(objTemp); Serial.println("*C");
  Serial.println();
  delay(500);
}
