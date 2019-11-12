#include <SoftwareSerial.h> 
SoftwareSerial chat(0,1);

int pirPin1 = 2;
int pirPin2 = 3; 

int pirState1;
int pirState2;
int i;

void setup() {
  // put your setup code here, to run once:
  chat.begin(4800);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  pirState1 = analogRead(pirPin1);
  pirState2 = analogRead(pirPin2);
  Serial.print("pirState1 : ");
  Serial.println(pirState1);
  Serial.print("pirState2 : ");
  Serial.println(pirState2);
  if(chat.available()>0){
    if(chat.readString() == "Pir1"){
      chat.print(pirState1);
    }
    if(chat.readString() == "Pir2"){
      chat.print(pirState2);
    }
    Serial.print("Send = ");
    Serial.println(i);
  }
  i++;
  delay(100);
}
