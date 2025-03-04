#include <SPI.h>
#include <Pixy2.h>
#include <PIDLoop.h>
Pixy2 pixy;

// Pin per il controllo dei motori
#define IN1 7
#define IN2 6
#define ENA 5
#define IN3 10
#define IN4 9
#define ENB 8

int cont = 0;
int signature, x, y, width, height;
float cx, cy, area;

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

void loop() 
{
  int i;
  int x_pos; // Posizione orizzontale dell'oggetto
  
  // Ottieni i blocchi dalla Pixy2
  pixy.ccc.getBlocks();

  // Se ci sono blocchi rilevati, stampali!
  if (pixy.ccc.numBlocks) 
  {
    Serial.print("Detected ");
    //Serial.println(pixy.ccc.numBlocks);

    for (i = 0; i < pixy.ccc.numBlocks; i++) {
      Serial.print("block ");
      Serial.print(i);
      Serial.print(": ");
      pixy.ccc.blocks[i].print();

      // Ottieni la posizione x del blocco
      x_pos = pixy.ccc.blocks[i].m_x;
      //Serial.println(x_pos); // Stampa la posizione x
    }

    // Controllo dei motori basato sulla posizione di x dell'oggetto 136 184
    //Serial.println("primadellif");
    //Serial.println(x_pos); 

    if (x_pos >= 136 && x_pos <= 184) {
      moveForward();  // I motori vanno avanti
    } 
    else if (x_pos < 136) {
       moveLeft();  // I motori si spostano verso sinistra
    } 
    else if (x_pos > 184) {
       moveRight();  // I motori si spostano verso destra
    }
  } 
  else {
    // Se nessun blocco viene rilevato, fermiamo i motori
    stopMotors();
  }
}

// Funzione per far andare il robot in avanti
void moveForward() {
  analogWrite(ENA, 200);  // Velocità 50% per il motore 1
  analogWrite(ENB, 200);  // Velocità 50% per il motore 2
  digitalWrite(IN1, HIGH); // Motore 1 in avanti
  digitalWrite(IN2, LOW);  // Motore 1 in avanti
  digitalWrite(IN3, HIGH); // Motore 2 in avanti
  digitalWrite(IN4, LOW);  // Motore 2 in avanti
}

// Funzione per far andare il robot a sinistra
void moveLeft() {
  analogWrite(ENA, 200);  // Velocità 50% per il motore 1
  analogWrite(ENB, 128);  // Velocità 50% per il motore 2
  digitalWrite(IN1, HIGH);  // Motore 1 indietro
  digitalWrite(IN2, LOW); // Motore 1 indietro
  digitalWrite(IN3, HIGH); // Motore 2 in avanti
  digitalWrite(IN4, LOW);  // Motore 2 in avanti
}

// Funzione per far andare il robot a destra
void moveRight() {
  analogWrite(ENA, 128);  // Velocità 50% per il motore 1
  analogWrite(ENB, 200);  // Velocità 50% per il motore 2
  digitalWrite(IN1, HIGH); // Motore 1 in avanti
  digitalWrite(IN2, LOW);  // Motore 1 in avanti
  digitalWrite(IN3, HIGH);  // Motore 2 indietro
  digitalWrite(IN4, LOW); // Motore 2 indietro
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
