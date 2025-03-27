import plasma
from plasma import plasma_stick
import time

"""
Make some rainbows!
"""

# Set how many LEDs you have
NUM_LEDS = 30

# The SPEED that the LEDs cycle at (1 - 255)
SPEED = 4

# How many times the LEDs will be updated per second


# WS2812 / NeoPixel™ LEDs
led_strip = plasma.WS2812(NUM_LEDS, 0, 0, plasma_stick.DAT, color_order=plasma.COLOR_ORDER_RGB)

# Start updating the LED strip
led_strip.start()

for i in range(NUM_LEDS):
        #green
        led_strip.set_hsv(i, 0.0, 1.0, 1.0)
        #red
        #led_strip.set_hsv(i, 0.3, 1.0, 1.0)        

time.sleep(65.0) # Per cambiare il tempo che ci vuole per passare dal verde al rosso
for i in range(NUM_LEDS):
        #red
        led_strip.set_hsv(i, 0.3, 1.0, 1.0)    
