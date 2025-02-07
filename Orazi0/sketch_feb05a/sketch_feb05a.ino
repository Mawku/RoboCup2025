#define IN1 5
#define IN2 4
#define ENA 6
#define IN3 7
#define IN4 8
#define ENB 9
#define TRIG_PIN 2
#define ECHO_PIN 3


 void stopProgram() {
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  analogWrite(10, 0);
  delay(999999999);  
} 
 void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);  
  Serial.begin(9600);       
  pinMode(TRIG_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT);  

}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long durata = pulseIn(ECHO_PIN, HIGH);  // Misura il tempo dell'eco
  int distanza = durata * 0.034 / 2;      // Conversione in cm
  delay(100);
  Serial.print("Distanza: ");
  Serial.print(distanza);
  Serial.println(" cm");
   int muro = 3;
if(distanza != muro){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 150);  // Velocità 0-255
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 150);  // Velocità 0-255
  }
  stopProgram();
  
  
  /* Motore indietro
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 150);
  */
}
