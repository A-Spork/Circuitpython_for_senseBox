#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h> 
#include <senseBoxIO.h>
#include <SparkFun_SCD30_Arduino_Library.h> 

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
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
 Wire.begin();
if (airSensor.begin() == false)
{
  while (1)
    ;
}
airSensor.useStaleData(true);
}

void loop() {
  display.clearDisplay();
   printOnDisplay("CO2", String(airSensor.getCO2()), "ppm", "Tmp", String(airSensor.getTemperature()), "°C");
   printOnDisplay("rH", String(airSensor.getHumidity()), "%", 0, String(), 0);
  delay(1000);
display.display();

}
