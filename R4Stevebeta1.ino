#include <Pixy2.h>
Pixy2 pixy;

int signature = 0;

void setup() {
  Serial.begin(115200); 
  pixy.init();  
  Serial.print("PixyCam2 inizializzata");
} 

void checkObject() {
  pixy.ccc.getBlocks();  

  if (pixy.ccc.numBlocks > 0) {
    signature = pixy.ccc.blocks[0].m_signature; // Prende la signature dell'oggetto

    if (signature == 1) {
      Serial.print("Oggetto rilevato con signature 1");
    }
  }
}

void loop() {
  checkObject();
  delay(100); // Piccola pausa per evitare letture troppo frequenti
}
