# Usecase 1
import board
import digitalio
import time

led = digitalio.DigitalInOut(board.LED)

def setup():
    led.direction = digitalio.Direction.OUTPUT
    led.value = True
    time.sleep(1)
    led.value = False
    time.sleep(1)
    led.value = True
    time.sleep(1)
    led.value = False

def loop():
    pass

setup()
while True:
    loop()
