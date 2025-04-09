#include <Pixy2.h>
#include <SoftwareSerial.h>

Pixy2 pixy;

int signature = 0;
const int distanza = 140; // Distanza da considerare per l'invio del messaggio
const int bluetoothRxPin = 10;  // Arduino RX <- HC-05 TX
const int bluetoothTxPin = 9;   // Arduino TX -> HC-05 RX (con partitore!)

SoftwareSerial bluetoothSerial(bluetoothRxPin, bluetoothTxPin); // RX, TX

void setup() {
  Serial.begin(9600); // Seriale per debug su PC
  Serial.println("Arduino (Master HC-05) pronto.");
  
  bluetoothSerial.begin(38400); // DEVE corrispondere a AT+UART=...
  Serial.println("Comunicazione Bluetooth avviata a 9600 baud.");
  
  pixy.init();  
}

void objectFollowing() {
  pixy.ccc.getBlocks();  

  signature = pixy.ccc.blocks[0].m_signature; // Ottiene la signature del blocco dalla Pixycam

  if (signature == 2) { // Se il blocco rilevato ha la signature 1
    int x, width;
    if (pixy.ccc.numBlocks) {
      x = pixy.ccc.blocks[0].m_x;
      width = pixy.ccc.blocks[0].m_width;

      if (x < distanza) {  // Se il blocco è a meno di "distanza" pixels
        Serial.println("Blocco rilevato. Inizio invio messaggio...");
        
        // Invio del messaggio iniziale
        bluetoothSerial.println("Messaggio dal Pixy!"); // Invia messaggio
        Serial.println("Messaggio inviato, attendo OK...");
        
        // Ciclo fino a che non ricevo "OK" o supero il timeout
        unsigned long startMillis = millis(); // Per il timeout
        bool receivedOK = false;
        while (millis() - startMillis < 5000) { // Timeout di 5 secondi
          if (bluetoothSerial.available()) {
            char dataFromOtherDevice = bluetoothSerial.read();
            Serial.print("Ricevuto: ");
            Serial.write(dataFromOtherDevice);
            Serial.println();

            // Controlla se riceve "OK" (prima 'O' e poi 'K')
            if (dataFromOtherDevice == 'O') {
              if (bluetoothSerial.available()) {
                dataFromOtherDevice = bluetoothSerial.read();
                if (dataFromOtherDevice == 'K') {
                  receivedOK = true; // Ricevuto OK
                  Serial.println("Ricevuto OK, invio messaggio interrotto.");
                  break; // Esce dal ciclo
                }
              }
            }
          }
        }

        // Se dopo 5 secondi non riceve "OK", invia il messaggio di errore
        if (!receivedOK) {
          Serial.println("Timeout: non ricevuto OK.");
        }
      }
    }
  }
}

void loop() {
  objectFollowing();
}
