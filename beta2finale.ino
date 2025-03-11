#include <Pixy2.h>
Pixy2 pixy;

int signature = 0;

//motor controller pins
#define ENA 5
#define ENB 8
#define IN1 7
#define IN2 8
#define IN3 10
#define IN4 9
#define carSpeed 200
#define carSpeed2 200
int rightDistance = 0, leftDistance = 0;

void forward(){ 
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Forward");
}

void back() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Back");
}


void left() {
  analogWrite(ENA, carSpeed2);
  analogWrite(ENB, carSpeed2);
  //digitalWrite(IN1, HIGH);
  //digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW); 
  Serial.println("left");
}

void right() {
  analogWrite(ENA, carSpeed2);
  analogWrite(ENB, carSpeed2);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  //digitalWrite(IN3, LOW);
  //digitalWrite(IN4, HIGH);
  Serial.println("right");
}

void stop() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}  

void setup() {
  Serial.begin(115200); 
//  transmitter.begin(9600);
  pixy.init();     
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

} 

void objectFollowing() {
  pixy.ccc.getBlocks();
  
  signature = pixy.ccc.blocks[0].m_signature;    //get object's signature

  if((signature == 1))
  {
    int x, width;
    if (pixy.ccc.numBlocks) {
        x = pixy.ccc.blocks[0].m_x;
        width = pixy.ccc.blocks[0].m_width;
        
     if (x < 85) left();
     else if (x > 235) right();
     else if (width < 150) forward();
     else if (width > 70) back();
     else  stop();
  } else
    stop();
}
}

void loop() { 
  
  objectFollowing();
} 
