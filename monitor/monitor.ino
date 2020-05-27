
// Arduino micro 
// implementação de monitoramento de plantas e clima local, interno e externo ao ambiente
// Equipamento:
// Higrometros - 2 média das plantas
// Sensor DHT22 - uso externo
// Sensor DHT11 - uso Interno
// Sensor de chuva - uso externo
// Sensor de Luminosidade - uso externo

// Modo de saida de dados: Serial, JSON
// Modo captação de dados: Raspberry pi / Node-Red / MongoDB

// dht lib e defines
#include "DHT.h"
#define DHTPIN11 2 // Digital pin DHT11 
#define DHTPIN22 3 // Digital pin DHT22 utiliza resistor de 10k (positivo - PIN)
#define DHTTYPE11 DHT22
#define DHTTYPE22 DHT22 
DHT dht11(DHTPIN11, DHTTYPE11);
DHT dht22(DHTPIN22, DHTTYPE22);

// LDR Sensores de luminosidade, ligacao (negativo - 10k - PIN)
int pinoSensorLuzI = A0;               
int valorLuzI = 0; 
int pinoSensorLuzE = A1;               
int valorLuzE = 0; 

// Sensores higrometro
byte humidity_sensor_pin_H1 = A2;
byte humidity_sensor_pin_H2 = A3;

byte chuva_pin = A4;

void setup() {
  Serial.begin(9600);
  dht11.begin();
  dht22.begin();
}

void loop() {
  delay(2000);
  float h11 = dht11.readHumidity();
  float t11 = dht11.readTemperature();
  float hic11 = dht11.computeHeatIndex(t11, h11, false);
  
  float h22 = dht22.readHumidity();
  float t22 = dht22.readTemperature();
  float hic22 = dht22.computeHeatIndex(t22, h22, false);

  Serial.print("{");
  if (isnan(h11) || isnan(t11) ) {
    Serial.print(F("\"dht11\":{\"humidade\":0,\"temperatura\":0,\"temperatura_sensacao\":0},"));
  }else{
    Serial.print("\"dht11\":{\"humidade\":");
    Serial.print(h11);
    Serial.print(",\"temperatura\":");
    Serial.print(t11);
    Serial.print(",\"temperatura_sensacao\":");
    Serial.print(hic11);
    Serial.print("},");
  }
  
  if (isnan(h22) || isnan(t22) ) {
    Serial.print(F("\"dht122\":{\"humidade\":0,\"temperatura\":0,\"temperatura_sensacao\":0},"));
  }else{
    Serial.print("\"dht22\":{\"humidade\":");
    Serial.print(h22);
    Serial.print(",\"temperatura\":");
    Serial.print(t22);
    Serial.print(",\"temperatura_sensacao\":");
    Serial.print(hic22);
    Serial.print("},");
  }
  valorLuzI = analogRead(pinoSensorLuzI);        
  Serial.print("\"luminosidade\":{\"interna\":");
  Serial.print(valorLuzI);
  valorLuzE = analogRead(pinoSensorLuzE);        
  Serial.print(",\"externa\":");
  Serial.print(valorLuzE);
  Serial.print("},");

     
  int valorH1 = analogRead(humidity_sensor_pin_H1);
  Serial.print("\"higrometro\":{\"H1\":");
  Serial.print(1023 - valorH1);
  int valorH2 = analogRead(humidity_sensor_pin_H2); 
  Serial.print(",\"H2\":");
  Serial.print(1023 - valorH2);
  Serial.print("}");

  int valorChuva = analogRead(chuva_pin);
  Serial.print(", \"chuva\":");
  Serial.print(1023 - valorChuva);
 


  
  
  Serial.print("}");
  Serial.println("");
}
