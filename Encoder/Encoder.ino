#include <PinChangeInterrupt.h>  // Necessaria per utilizzare interrupt su pin non nativi

// === DEFINIZIONE PIN DRIVER MOTORE ===
// Motore 1
#define IN1 5
#define IN2 4
#define ENA 10

// Motore 2
#define IN3 7
#define IN4 8
#define ENB 6

// === PIN ULTRASONICO ===
#define TRIG_PIN 2
#define ECHO_PIN 3

// === PIN ENCODER ===
// Motore 1 Encoder
const int encoderA = 11;   // Canale A del motore 1
const int encoderB = 12;   // Canale B del motore 1

// Motore 2 Encoder
const int encoderC = 1;    // Canale A del motore 2 (da qui in poi, useremo anche il canale D)
const int encoderD = 13;   // Canale B del motore 2

// === VARIABILI ENCODER E PID ===
// Motore 1
volatile long encoderCounts1 = 0;
volatile int direction1 = 0; // 1 = avanti, -1 = indietro

// Motore 2
volatile long encoderCounts2 = 0;
volatile int direction2 = 0; // 1 = avanti, -1 = indietro

// Parametri PID comuni (modifica i parametri per ogni motore se necessario)
// Motore 1
double setpoint1 = 100;   // Target impulsi per intervallo
double input1 = 0;
double output1 = 0;
double Kp1 = 2.0, Ki1 = 0.5, Kd1 = 0.1;
double lastError1 = 0;
double integral1 = 0;

// Motore 2
double setpoint2 = 100;   // Target impulsi per intervallo
double input2 = 0;
double output2 = 0;
double Kp2 = 2.0, Ki2 = 0.5, Kd2 = 0.1;
double lastError2 = 0;
double integral2 = 0;

// Timing per PID
unsigned long lastTime = 0;
unsigned long sampleTime = 100; // millisecondi

// Soglia per attivare il motore (in uscita digitale)
const int digitalThreshold = 127;

// === FUNZIONE DI STOP PER ENTRAMBI I MOTORI ===
void stopProgram() {
  // Ferma motore 1
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
  // Ferma motore 2
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
  Serial.println("STOP - Ostacolo rilevato!");
  while(1);  // Blocca il programma
}

// === ISR ENCODER ===
// Per motore 1: ISR per encoderA
void encoderA_ISR() {
  // Se il valore di encoderA è uguale a encoderB, si incrementa; altrimenti si decrementa
  if (digitalRead(encoderA) == digitalRead(encoderB)) {
    encoderCounts1++;
    direction1 = 1;
  } else {
    encoderCounts1--;
    direction1 = -1;
  }
}

// Per motore 2: utilizziamo encoderC come trigger
void encoderC_ISR() {
  if (digitalRead(encoderC) == digitalRead(encoderD)) {
    encoderCounts2++;
    direction2 = 1;
  } else {
    encoderCounts2--;
    direction2 = -1;
  }
}

void setup() {
  // Configurazione pin encoder
  pinMode(encoderA, INPUT);
  pinMode(encoderB, INPUT);
  pinMode(encoderC, INPUT);
  pinMode(encoderD, INPUT);
  
  // Configurazione pin driver motori
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  // Configurazione ultrasuoni
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  Serial.begin(9600);
  
  // Attacca interrupt per motore 1 (encoderA) e motore 2 (encoderC)
  attachPCINT(digitalPinToPCINT(encoderA), encoderA_ISR, CHANGE);
  attachPCINT(digitalPinToPCINT(encoderC), encoderC_ISR, CHANGE);
  
  // Imposta direzione iniziale dei motori (ad es. avanti)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
  lastTime = millis();
}

void loop() {
  unsigned long now = millis();
  
  // Legge il sensore ultrasonico ad ogni ciclo PID
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long durata = pulseIn(ECHO_PIN, HIGH);  // Tempo di eco in microsecondi
  int distanza = durata * 0.034 / 2;         // Conversione in centimetri
  
  Serial.print("Distanza: ");
  Serial.print(distanza);
  Serial.println(" cm");
  
  int muro = 3;  // Distanza in cm alla quale fermarsi
  if(distanza <= muro) {   // Se rileviamo un ostacolo troppo vicino
    stopProgram();
  }
  
  // Esegui il ciclo PID ogni sampleTime millisecondi
  if (now - lastTime >= sampleTime) {
    // ----- MOTORE 1 -----
    noInterrupts();
    long count1 = encoderCounts1;
    encoderCounts1 = 0;
    interrupts();
    input1 = count1;
    double error1 = setpoint1 - input1;
    integral1 += error1 * ((now - lastTime) / 1000.0);
    double derivative1 = (error1 - lastError1) / ((now - lastTime) / 1000.0);
    output1 = Kp1 * error1 + Ki1 * integral1 + Kd1 * derivative1;
    if (output1 > 255) output1 = 255;
    if (output1 < 0) output1 = 0;
    lastError1 = error1;
    
    // Controllo digitale per motore 1
    if (output1 > digitalThreshold) {
      // Attiva motore 1 (in avanti) con velocità fissata (es. 100)
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, 100);
    } else {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, 0);
    }
    
    // ----- MOTORE 2 -----
    noInterrupts();
    long count2 = encoderCounts2;
    encoderCounts2 = 0;
    interrupts();
    input2 = count2;
    double error2 = setpoint2 - input2;
    integral2 += error2 * ((now - lastTime) / 1000.0);
    double derivative2 = (error2 - lastError2) / ((now - lastTime) / 1000.0);
    output2 = Kp2 * error2 + Ki2 * integral2 + Kd2 * derivative2;
    if (output2 > 255) output2 = 255;
    if (output2 < 0) output2 = 0;
    lastError2 = error2;
    
    // Controllo digitale per motore 2
    if (output2 > digitalThreshold) {
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, 120);
    } else {
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, 0);
    }
    
    // Debug: stampa i dati per entrambi i motori
    Serial.print("Motore 1 -> Impulsi: ");
    Serial.print(input1);
    Serial.print(" | Setpoint: ");
    Serial.print(setpoint1);
    Serial.print(" | Output: ");
    Serial.print(output1);
    Serial.print(" | Dir: ");
    Serial.print(direction1 == 1 ? "Avanti" : (direction1 == -1 ? "Indietro" : "Fermo"));
    Serial.print(" || ");
    Serial.print("Motore 2 -> Impulsi: ");
    Serial.print(input2);
    Serial.print(" | Setpoint: ");
    Serial.print(setpoint2);
    Serial.print(" | Output: ");
    Serial.print(output2);
    Serial.print(" | Dir: ");
    Serial.println(direction2 == 1 ? "Avanti" : (direction2 == -1 ? "Indietro" : "Fermo"));
    
    lastTime = now;
  }
}
