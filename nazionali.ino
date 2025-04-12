#include <Pixy2.h>
#define IN1 9
#define IN2 8
#define ENA 10
#define IN3 7
#define IN4 6
#define ENB 5

Pixy2 pixy;

void muovi(String direzione) {
  if (direzione == "avanti") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 200);
    analogWrite(ENB, 200);
  } else if (direzione == "destra") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 200);
    analogWrite(ENB, 0);
  } else if (direzione == "sinistra") {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 200);
  }
}

void muovi1(String direzione) {
  if (direzione == "avanti") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 100);
    analogWrite(ENB, 100);
  } else if (direzione == "destra") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 100);
    analogWrite(ENB, 0);
  } else if (direzione == "sinistra") {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 100);
  }
}

void ferma() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void setup() {
  Serial.begin(9600);
  pixy.init();
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() {
  ferma();
  pixy.ccc.getBlocks();

  if (pixy.ccc.numBlocks) {
    int x = pixy.ccc.blocks[0].m_x;
    int altezza = pixy.ccc.blocks[0].m_height;

    if (altezza > 160) {
      Serial.println("Sto per sbattere Mbare, Fermi tutti");
      ferma();
    } else if (altezza > 100) {
      Serial.println("Troppo vicino mbare, prendiamola con calma");
      if (x < 100) {
        Serial.println("Zombie a sinistra");
        muovi1("sinistra");
      } else if (x > 216) {
        Serial.println("Zombie a destra");
        muovi1("destra");
      } else {
        Serial.println("Zombie al centro");
        muovi1("avanti");
      }
    } else {
      if (x < 100) {
        Serial.println("Zombie a sinistra");
        muovi("sinistra");
      } else if (x > 216) {
        Serial.println("Zombie a destra");
        muovi("destra");
      } else {
        Serial.println("Zombie al centro");
        muovi("avanti");
      }
    }

    delay(200);
  }
}
