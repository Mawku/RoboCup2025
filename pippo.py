import plasma
from plasma import plasma_stick
import time

"""
Make some rainbows!
"""

NUM_LEDS = 30
 
SPEED = 4

led_strip = plasma.WS2812(NUM_LEDS, 0, 0, plasma_stick.DAT, color_order=plasma.COLOR_ORDER_RGB)

led_strip.start()

for i in range(NUM_LEDS):
       
        led_strip.set_hsv(i, 0.0, 1.0, 1.0)
       
time.sleep(65.0) 
for i in range(NUM_LEDS):
        
        led_strip.set_hsv(i, 0.3, 1.0, 1.0)    
