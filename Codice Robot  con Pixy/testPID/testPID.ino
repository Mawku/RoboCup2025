#include <SPI.h>
#include <Pixy2.h>

// Pin per il controllo dei motori
#define IN1 7
#define IN2 6
#define ENA 5
#define IN3 10
#define IN4 9
#define ENB 8

Pixy2 pixy;

// Parametri PID
float Kp = 0.5;  // Guadagno proporzionale
float Ki = 0.1;  // Guadagno integrale
float Kd = 0.05; // Guadagno derivativo

float previous_error = 0;  // Errore precedente
float integral = 0;        // Somma degli errori
int target = 160;          // Posizione desiderata dell'oggetto (centro dell'immagine)
int target_distance = 50;  // Distanza ideale per "toccare" l'oggetto (valore da regolare)

void setup() {
  Serial.flush();
  Serial.begin(9600);
  Serial.print("Starting...\n");

  // Imposta i pin come OUTPUT
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Inizializza Pixy2
  pixy.init();
}

void loop() {
  int i;
  int x_pos = 0; // Posizione orizzontale dell'oggetto
  int y_pos = 0; // Posizione verticale dell'oggetto
  int width = 0; // Larghezza dell'oggetto
  int height = 0; // Altezza dell'oggetto

  // Ottieni i blocchi dalla Pixy2
  pixy.ccc.getBlocks();

  // Se ci sono blocchi rilevati, stampali!
  if (pixy.ccc.numBlocks) {
    Serial.print("Detected ");
    
    for (i = 0; i < pixy.ccc.numBlocks; i++) {
      Serial.print("block ");
      Serial.print(i);
      Serial.print(": ");
      pixy.ccc.blocks[i].print();

      // Ottieni la posizione x e y del blocco
      x_pos = pixy.ccc.blocks[i].m_x;
      y_pos = pixy.ccc.blocks[i].m_y;
      width = pixy.ccc.blocks[i].m_width;
      height = pixy.ccc.blocks[i].m_height;
    }

    // Calcola l'errore tra la posizione dell'oggetto e il centro
    float error = target - x_pos;

    // Calcola la distanza tra l'oggetto e il centro (idealmente vogliamo avvicinarci)
    int distance_error = abs(target_distance - width); // Convertilo in un errore di distanza

    // Calcola l'integrale (somma degli errori passati)
    integral += error;

    // Calcola la derivata (variazione dell'errore)
    float derivative = error - previous_error;

    // Calcola la correzione PID per la direzione
    float correction = Kp * error + Ki * integral + Kd * derivative;

    // Correzioni per il movimento del robot
    if (correction == 0 && distance_error < 10) {
      moveForward();  // Robot va dritto e avanza verso l'oggetto
    } else {
      // Correzione per la direzione
      if (correction > 0) {
        moveRight(abs(correction));  // Corregge la direzione verso destra
      }
      else if (correction < 0) {
        moveLeft(abs(correction));  // Corregge la direzione verso sinistra
      }

      // Se l'oggetto è troppo vicino o troppo lontano, regolare la velocità
      if (distance_error > 10) {
        // Se la distanza è troppo grande, avvicinarsi all'oggetto
        moveForward(); 
      }
      else {
        // Se l'oggetto è abbastanza vicino, rallentare o fermarsi
        stopMotors();
      }
    }

    // Memorizza l'errore per il calcolo del derivato nel prossimo ciclo
    previous_error = error;

  } else {
    // Se nessun blocco viene rilevato, fermiamo i motori
    stopMotors();
  }
}

// Funzione per far andare il robot in avanti
void moveForward() {
  analogWrite(ENA, 128);  // Velocità 50% per il motore 1
  analogWrite(ENB, 128);  // Velocità 50% per il motore 2
  digitalWrite(IN1, HIGH); // Motore 1 in avanti
  digitalWrite(IN2, LOW);  // Motore 1 in avanti
  digitalWrite(IN3, HIGH); // Motore 2 in avanti
  digitalWrite(IN4, LOW);  // Motore 2 in avanti
}

// Funzione per far andare il robot a sinistra
void moveLeft(int speed) {
  analogWrite(ENA, speed);  // Velocità del motore 1
  analogWrite(ENB, speed);  // Velocità del motore 2
  digitalWrite(IN1, LOW);   // Motore 1 indietro
  digitalWrite(IN2, HIGH);  // Motore 1 indietro
  digitalWrite(IN3, HIGH);  // Motore 2 in avanti
  digitalWrite(IN4, LOW);   // Motore 2 in avanti
}

// Funzione per far andare il robot a destra
void moveRight(int speed) {
  analogWrite(ENA, speed);  // Velocità del motore 1
  analogWrite(ENB, speed);  // Velocità del motore 2
  digitalWrite(IN1, HIGH);  // Motore 1 in avanti
  digitalWrite(IN2, LOW);   // Motore 1 in avanti
  digitalWrite(IN3, LOW);   // Motore 2 indietro
  digitalWrite(IN4, HIGH);  // Motore 2 indietro
}

// Funzione per fermare i motori
void stopMotors() {
  analogWrite(ENA, 0);  // Fermiamo il motore 1
  analogWrite(ENB, 0);  // Fermiamo il motore 2
  digitalWrite(IN1, LOW); // Fermiamo motore 1
  digitalWrite(IN2, LOW); // Fermiamo motore 1
  digitalWrite(IN3, LOW); // Fermiamo motore 2
  digitalWrite(IN4, LOW); // Fermiamo motore 2
}
