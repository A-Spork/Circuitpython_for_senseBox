# Usecase 4
import board
import displayio
import adafruit_displayio_ssd1306
import terminalio
from adafruit_display_text import label
import time
import adafruit_scd30
import adafruit_dotstar

displayio.release_displays()
WIDTH = 128
HEIGHT = 64
i2c = board.STEMMA_I2C()
display_bus = displayio.I2CDisplay(i2c, device_address=0x3D)
display = adafruit_displayio_ssd1306.SSD1306(display_bus, width=WIDTH, height=HEIGHT)
scd = adafruit_scd30.SCD30(i2c)
splash = displayio.Group()
text_area = label.Label(
    terminalio.FONT, text="", x=12, color=0xFFFFFF, y=15
)
splash.append(text_area)
pixels = adafruit_dotstar.DotStar(board.APA102_SCK, board.APA102_MOSI, 1)

def printOnDisplay(title1, measurement1, unit1, title2, measurement2, unit2):
    text = title1 + "\n" + str(measurement1) + unit1 + "\n" + title2 + "\n" + str(measurement2) + unit2
    text_area.text = text

def setup():
    display.show(splash)

def loop():
    if scd.CO2 < 1000:
        pixels[0] = (0, 255, 0, 0.1)
    elif scd.CO2 < 1500:
        pixels[0] = (255, 255, 0 , 0.1)
    else:
        pixels[0] = (255, 0, 0, 0.1)
    printOnDisplay("CO2", str(scd.CO2), "ppm", "", "", "")
    time.sleep(2)

setup()
while True:
    loop()
