# Usecase 2
import board
import time
import adafruit_dotstar

pixels = adafruit_dotstar.DotStar(board.APA102_SCK, board.APA102_MOSI, 1)

def setup():
   pass

def loop():
    pixels[0] = (0, 255, 0, 0.1)
    time.sleep(1)
    pixels[0] = (255, 0, 0, 0.1)
    time.sleep(1)
    pixels[0] = (125, 125, 0, 0.1)
    time.sleep(1)

setup()
while True:
    loop()
