import machine
import neopixel
import time
import uos

NUM_LEDS = 29  # Numero totale di LED nella striscia
LED_PIN = machine.Pin(2)  # Usa GP2 per i dati dei LED

# Inizializza la striscia LED
led_strip = neopixel.NeoPixel(LED_PIN, NUM_LEDS)
uart = machine.UART(0, baudrate=9600, tx=machine.Pin(0), rx=machine.Pin(1))

# Funzione per accendere tutti i LED di verde
def accendi_led_verdi():
    for i in range(NUM_LEDS):
        led_strip[i] = (0, 255, 0)  # Verde (RGB: 0, 255, 0)
    led_strip.write()  # Applica le modifiche

# Funzione per accendere tutti i LED di rosso
def accendi_led_rossi():
    for i in range(NUM_LEDS):
        led_strip[i] = (255, 0, 0)  # Rosso (RGB: 255, 0, 0)
    led_strip.write()  # Applica le modifiche

def spegni_led():
    for i in range(NUM_LEDS):
        led_strip[i] = (0, 0, 0)  # Spegni tutti i LED
    led_strip.write()

# Accendi i LED verdi all'avvio
accendi_led_verdi()
print("Pico W (MASTER HC-05) pronto.")
message_buffer = ""

while True:
    # Verifica se ci sono dati disponibili sulla UART
    if uart.any():
        try:
            data_from_arduino = uart.read(1)  # Leggi un byte alla volta
            
            # Se i dati ricevuti sono validi (ad esempio, un carattere alfanumerico)
            if data_from_arduino and (data_from_arduino.isalpha() or data_from_arduino.isdigit()):
                accendi_led_rossi()  # Accende i LED di rosso per indicare che il dato è ricevuto
                uart.write("OK\n")  # Invia conferma
                print("Ricevuto da Arduino: ", data_from_arduino.decode('utf-8'), end='')
            else:
                # Se i dati non sono validi (rumore o dati non alfanumerici), ignora
                print("Ricevuto byte non valido o rumore sulla UART:", data_from_arduino)

        except Exception as e:
            print("Errore nella lettura dei dati UART:", e)
    
    # Aspetta un po' prima di continuare
    time.sleep(2)
    time.sleep_ms(10)  # Piccolo delay per non sovraccaricare la CPU
