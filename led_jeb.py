import board
import neopixel
import time

# Configurazione
LED_PIN = board.GP0  # Pin dati LED
NUM_PIXELS = 30      # Numero di LED nella striscia
BRIGHTNESS = 0.5     # Luminosità (0.0 - 1.0)
# The SPEED that the LEDs cycle at (1 - 255)
SPEED = 4

# How many times the LEDs will be updated per second
UPDATES = 100
offset = 0.0
# Inizializza la striscia LED
pixels = neopixel.NeoPixel(LED_PIN, NUM_PIXELS, brightness=BRIGHTNESS, auto_write=False)

def rainbow_cycle(wait,j):
        global SPEED,offset,hue
        SPEED = min(255, max(1, SPEED))
        offset += float(SPEED) / 2000.0
        offset %= 1
        hue = offset
        pixel_index = (256 * j) // 255
    #for j in range(255):
        for i in range(NUM_PIXELS):
            
            """+ j"""
            pixels[i] = wheel(pixel_index)
        pixels.show()
        time.sleep(wait)

def wheel(pos):
   # Genera colori arcobaleno a seconda della posizione nella gamma 0-255.
    if pos < 85:
        return (pos * 3, 255 - pos * 3, 0)
    elif pos < 170:
        pos -= 85
        return (255 - pos * 3, 0, pos * 3)
    else:
        pos -= 170 
        return (0, pos * 3, 0)

while True:
    for j in range(255):
        rainbow_cycle(1.0/UPDATES,j)  # Effetto arcobaleno
    

