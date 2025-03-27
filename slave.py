import machine
import utime

# Imposta il ruolo: "MASTER" per il trasmettitore, "SLAVE" pex1r il ricevitore
ROLE = "SLAVE"  # Cambia in "SLAVE" sul Pico ricevente

# Variabile booleana per l'invio del messaggio (1 = invia, 0 = non invia)
# Per test, puoi impostarla manualmente a 1 dalla REPL oppure modificarla nel codice
SEND_MSG = 0

# Configura UART per la comunicazione normale (9600 baud)
uart = machine.UART(0, baudrate=9600, tx=machine.Pin(0), rx=machine.Pin(1))

def invia_messaggio(msg):
    uart.write(msg + "\r\n")
    print("Messaggio inviato:", msg)

def ricevi_messaggio():
    if uart.any():
        msg = uart.readline()
        if msg:
            print("Messaggio ricevuto:", msg.decode().strip())

if ROLE == "MASTER":
    print("MASTER in attesa del comando per inviare il messaggio.")
    while True:
        # Se SEND_MSG viene impostato a 1, invia il messaggio e poi lo resetta a 0
        if SEND_MSG == 1:
            invia_messaggio("Ciao dal MASTER!")
            SEND_MSG = 0  # Reset per evitare invii ripetuti
        utime.sleep(0.1)

elif ROLE == "SLAVE":
    print("SLAVE in attesa di messaggi dal MASTER...")
    while True:
        ricevi_messaggio()
        utime.sleep(0.1)


