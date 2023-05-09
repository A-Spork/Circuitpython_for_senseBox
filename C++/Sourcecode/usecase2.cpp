#include <senseBoxIO.h>
#include <Adafruit_NeoPixel.h>
 Adafruit_NeoPixel rgb_led_1= Adafruit_NeoPixel(1, 1,NEO_GRB + NEO_KHZ800);

void setup() {
rgb_led_1.begin();
rgb_led_1.setBrightness(100);
}

void loop() {
rgb_led_1.setPixelColor(0,rgb_led_1.Color(100, 0, 0));
rgb_led_1.show();
delay(1000);
rgb_led_1.setPixelColor(0,rgb_led_1.Color(0, 100, 0));
rgb_led_1.show();
delay(1000);
rgb_led_1.setPixelColor(0,rgb_led_1.Color(0, 0, 100));
rgb_led_1.show();
delay(1000);

}