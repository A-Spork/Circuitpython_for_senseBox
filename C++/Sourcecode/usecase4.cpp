#include <Sigh>
#include <Wire.h>
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h> 
#include <senseBoxIO.h>
#include <SparkFun_SCD30_Arduino_Library.h> 
float CO2;
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#include <Adafruit_NeoPixel.h>
 Adafruit_NeoPixel rgb_led_6= Adafruit_NeoPixel(1, 6,NEO_GRB + NEO_KHZ800);
SCD30 airSensor;
    void printOnDisplay(String title1, String measurement1, String unit1, String title2, String measurement2, String unit2) {
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.setTextColor(WHITE, BLACK);
      display.println(title1);
      display.setCursor(0, 10);
      display.setTextSize(2);
      display.print(measurement1);
      display.print(" ");
      display.setTextSize(1);
      display.println(unit1);
      display.setCursor(0, 30);
      display.setTextSize(1);
      display.println(title2);
      display.setCursor(0, 40);
      display.setTextSize(2);
      display.print(measurement2);
      display.print(" ");
      display.setTextSize(1);
      display.println(unit2);
    }

void setup() {
senseBoxIO.powerI2C(true);
delay(2000);
display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
display.display();
delay(100);
display.clearDisplay();
rgb_led_6.begin();
rgb_led_6.setBrightness(30);
 Wire.begin();
if (airSensor.begin() == false)
{
  while (1)
    ;
}
airSensor.useStaleData(true);
}
void loop() {
CO2 = airSensor.getCO2();
if (( CO2 < 800)) {
  rgb_led_6.setPixelColor(0,rgb_led_6.Color(0, 153, 0));
  rgb_led_6.show();
}
 else if (( CO2 < 1500)) {
  rgb_led_6.setPixelColor(0,rgb_led_6.Color(255, 255, 153));
  rgb_led_6.show();
}
 else {
  rgb_led_6.setPixelColor(0,rgb_led_6.Color(255, 0, 0));
  rgb_led_6.show();
}
   printOnDisplay("CO2", String(CO2), "ppm", "", String(""), "");
display.display();
delay(1000);
display.clearDisplay();
}
