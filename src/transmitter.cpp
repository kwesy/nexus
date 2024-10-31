#include <transmitter.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 txRadio(7, 8); // CE, CSN

const byte address[6] = "00001";

void Transmitter::init() {
  txRadio.begin();
  txRadio.openWritingPipe(address);
  txRadio.setPALevel(RF24_PA_MIN);
  txRadio.stopListening();
}

void Transmitter::sendSignal(int *text) {
  if (!txRadio.available()) {
    Serial.println("Tx not available");
  }
  if (txRadio.available()) {
    txRadio.write(&text, sizeof(text));
    Serial.println("sending");
    Serial.println(*text);
    // txRadio.write(&channels, sizeof(channels));
    // Serial.println(channels.pitch);
  }
}
