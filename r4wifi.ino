#include <ArduinoBLE.h>

BLEService myService("180C");  // UUID del servizio
BLEStringCharacteristic myCharacteristic("2A56",  // UUID della caratteristica
                                         BLEWrite | BLERead, 20); // 20 caratteri max

void setup() {
    Serial.begin(115200);
    while (!Serial);

    if (!BLE.begin()) {
        Serial.println("Errore avvio BLE!");
        while (1);
    }

    BLE.setLocalName("ArduinoR4BLE");
    BLE.setAdvertisedService(myService);
    myService.addCharacteristic(myCharacteristic);
    BLE.addService(myService);
    BLE.advertise();

    Serial.println("Arduino BLE pronto a ricevere messaggi...");
}

void loop() {
    BLEDevice central = BLE.central();

    if (central) {
        Serial.print("Connesso a: ");
        Serial.println(central.address());

        while (central.connected()) {
            if (myCharacteristic.written()) {  // Controlla se Ã¨ stato scritto un messaggio
                String receivedMessage = myCharacteristic.value();
                Serial.print("Messaggio ricevuto: ");
                Serial.println(receivedMessage);
            }
        }

        Serial.println("Dispositivo disconnesso.");
    }
}