// Noah Törngren 12-13
#include "U8glib.h"
#include <Wire.h>

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NO_ACK);

int led = 13 ; // LED on arduino
int digitalPin = 3; // linear Hall magnetic sensor digital interface
int analogPin = A0; // linear Hall magnetic sensor analog interface
int digitalVal ; // digital readings
float analogVal; // analog readings


float ary[30]; //array för medelvädet av linear Hall magnetic sensor värderna

void setup() {
  Serial.begin(9600);
  pinMode (led, OUTPUT);
  pinMode (digitalPin, INPUT);

  Wire.begin();
  oled.setFont(u8g_font_helvB10);

  if (digitalVal == HIGH) // When magnetic field is present, Arduino LED is on
  {
    digitalWrite (led, HIGH);
  }
  else
  {
    digitalWrite (led, LOW);
  }


  // Read the analog interface
  analogVal = analogRead(analogPin);

  // fyller arrayen med det nuvarande värdet från magnetic hall sensor
  for (int i = 0; i < 30; i++) {
    ary[i] = analogVal;
  }
}

void loop() {

  // Read the digital interface
  digitalVal = digitalRead(digitalPin) ;
  if (digitalVal == HIGH) // When magnetic field is present, Arduino LED is on
  {
    digitalWrite (led, HIGH);
  }
  else
  {
    digitalWrite (led, LOW);
  }

  // Read the analog interface
  analogVal = analogRead(analogPin);

  updateAry(analogVal, ary, 30);
  Serial.println(adcToGauss(aryAverage(ary, 30)));

  updateOled(String(adcToGauss(aryAverage(ary, 30))));


}

/*
   Uppdaterar arrayen med värderna från magnetic hall sensorn. Tar in variablen value, sätter det som första värdet och flyttar alla andra värden
   med en position bakåt. Därför försvinner sista värdet

   Parametrar:
   value: det värdet som ska läggas till
   arg: den arrayen som värdet ska läggas till i
   sizeAry: storleken på arrayen som ska modifieras
   Return: inget
*/
void updateAry(float value, float arg[], int sizeAry) {
  for (int n = sizeAry - 1; n >= 1; n--) {
    arg[n] = arg[n - 1];
  }
  arg[0] = value;

}

/*
   Beräknar medelvärdet av alla värden i arg
   Parametrar:
   arg (float array som är alla värden som fåtts av linear Hall magnetic sensor)
   argSize (en int som beskriver storleken på arg)

   return: resultatet som är en float, alltså medelvärdet
*/
float aryAverage(float arg[], int argSize) {
  float number = 0;
  float result = 0;
  for (int i = 0; i <= 29; i++) {
    number = number + arg[i];

  }
  result = (number / argSize);
  return (result);
}

/*
   Gör om adc värdet som vi får från den analoga utputen. Gör om den till spännings värdet och sedan till gauss
   Parameter: adc(det analoga värdet som kommer från magnetic hall sensor, eller medelvärdet av de 30 senaste
   Return: Skickar tillbaka resultatet som är antalet gauss, styrkan på magnetfältet
*/
float adcToGauss(float adc) {
  // 667 * 0,0049 * adc - 1667
  return (3.2683 * adc) - 1667;

}

/*
   uppdaterar oled skärmen
   Parametrar: text(en string som är innehållet som ska skrivas ut)
   Rerurn: inget
*/
void updateOled(String text) {
  oled.firstPage();


  do {
    oled.drawStr(10, 20, text.c_str());
  } while (oled.nextPage());

}
