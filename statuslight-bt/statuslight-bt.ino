// Also known as Torchlight.

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define ONB 2

BluetoothSerial SerialBT;

uint32_t serial = 0;
int red = 13;
int ylw = 14;
int grn = 15;

String message = "";
char incomingChar;
String current = "";

void setup() {
  pinMode(red, OUTPUT);
  pinMode(ylw, OUTPUT);
  pinMode(grn, OUTPUT);
  pinMode(ONB, OUTPUT);
  
  digitalWrite(ONB, LOW);
  delay(250);
  digitalWrite(ONB, HIGH);
  delay(250);
  digitalWrite(ONB, LOW);
  delay(250);
  digitalWrite(ONB, HIGH);
  delay(250);
  digitalWrite(ONB, LOW);
  
  digitalWrite(red, LOW);
  digitalWrite(ylw, HIGH);
  digitalWrite(grn, LOW);
  current = "RING FIRST";
  
  for(int i=0; i<17; i=i+8) {
    serial |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  String btName = "Torchlight-"+String(serial);
  Serial.begin(115200);
  SerialBT.begin(btName); //Bluetooth device name
  Serial.print("Device started - ");
  Serial.println(btName);
}

void loop() {
if (SerialBT.available()){
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n'){
      message += String(incomingChar);
      digitalWrite(ONB, HIGH);
      delay(150);
      digitalWrite(ONB, LOW);
      delay(150);
    }
    else{
      message = "";
    }
    Serial.write(incomingChar);  
  
  // Check received message and control output accordingly
  if (message == "1"){
    digitalWrite(red, LOW);
    digitalWrite(ylw, LOW);
    digitalWrite(grn, HIGH);
    SerialBT.println("Set status to AVAILABLE");
    current = "AVAILABLE";
    message = "";
  }
  if (message == "2"){
    digitalWrite(red, LOW);
    digitalWrite(ylw, HIGH);
    digitalWrite(grn, LOW);
    SerialBT.println("Set status to RING FIRST");
    current = "RING FIRST";
    message = "";
  }
  if (message == "3"){
    digitalWrite(red, HIGH);
    digitalWrite(ylw, LOW);
    digitalWrite(grn, LOW);
    SerialBT.println("Set status to DO NOT DISTURB");
    current = "DO NOT DISTURB";
    message = "";
  }
  if (message == "4"){
    digitalWrite(red, LOW);
    digitalWrite(ylw, HIGH);
    digitalWrite(grn, HIGH);
    SerialBT.println("Set status to AVAILABLE - RING FIRST");
    current = "AVAILABLE - RING FIRST";
    message = "";
  }
  else {
    SerialBT.println("Valid string: 1, 2, 3 or 4");
    SerialBT.print("Current status: ");
    SerialBT.println(current);
    message = "";
  }
  delay(20);
  }
}
