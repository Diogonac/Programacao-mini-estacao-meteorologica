#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <LiquidCrystal_PCF8574.h>
#include <Adafruit_BMP085.h>

int ledPin1 = 9;
int ledPin2 = 10;
int ledPin3 = 11;

const int lcd_adress  = 0x27;
const int lcd_columns = 16;
const int lcd_rows    = 2;
LiquidCrystal_PCF8574 lcd(lcd_adress);

Adafruit_BMP085 bmp;

const int DHT_PIN = 2;
DHT dht(DHT_PIN, DHT22);

float umid, temp;

void setup() {
  Serial.begin(9600);
  dht.begin();

  lcd.begin(lcd_columns, lcd_rows);
  lcd.setBacklight(100);
  lcd.setCursor(0, 0);

  if (bmp.begin()) {
    Serial.println("OK: BMP Encontrado");
  }
  else {
    Serial.println("Erro: BMP não encontrado");
  }

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
}

void loop() {
  float pressao, temp_bmp;
  pressao = bmp.readPressure();
  float  pressao_Atm = pressao * 9.86923e-6;
  Serial.print("Pressão: ");
  Serial.println(pressao_Atm);
  float a = 0.00197385;

  float inBMP = sqrt(a * a);


  int leitura_adc = analogRead(A1);
  float voltagem = leitura_adc * (5.0f / 1023);
  float temperatura1 = voltagem * 100;
  float temperatura = (temperatura1 - 0.927377402)/0.9460616;
  Serial.print("Temperatura: ");
  Serial.println(temperatura, 3);
  float b = (5.0 / 1023.0);

  float inLM = sqrt((0.5) * (0.5) + b * b);


float  umid1 = dht.readHumidity();
  umid = (umid1 -15.44500441)/0.846123423;  
  Serial.print("Umidade: ");
  Serial.println(umid);

  float inDH = 2;


  int bin;
  double tensao_ldr, res, lum;
  bin = analogRead(A0);
  tensao_ldr = bin * 5.0 / 1023;
  res = 1000.0 * (5.0 / tensao_ldr + 1);
  lum = pow(10, 5 - log10(res));
  Serial.print("Luminosidade: ");
  Serial.println(lum);

  float altitude = (bmp.readAltitude(101500));
  Serial.print("Altitude: ");
  Serial.println(altitude);

  float inAlt = 0.17;

  lcd.setCursor(0, 0);
  lcd.print("Temperatura C");
  lcd.setCursor(0, 1);
  lcd.print(temperatura, 2); lcd.print(" +-"); lcd.print(inLM);
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Umidade %");
  lcd.setCursor(0, 1);
  lcd.print(umid, 2); lcd.print(" +-"); lcd.print(inDH);
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Pressao Atm");
  lcd.setCursor(0, 1);
  lcd.print(pressao_Atm, 2); lcd.print(" +-"); lcd.print(0.001);
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Altitude m");
  lcd.setCursor(0, 1);
  lcd.print(altitude, 2); lcd.print(" +-"); lcd.print(0.17);
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Luminosidade Lux");
  lcd.setCursor(0, 1);
  lcd.print(lum, 2);
  delay(2000);
  lcd.clear();

  float led = mapfloat(lum, 45.15, 4.21, 10, 255);
  Serial.println();
  Serial.println(led);
  Serial.println();
  analogWrite(11, led);
  analogWrite(10, led);
  analogWrite(9, led);

}
float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}
