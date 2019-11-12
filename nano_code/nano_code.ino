#define buzzer 8
#define led 7
void setup() {

  Serial.begin(115200);
  pinMode(led,OUTPUT);
  pinMode(buzzer,OUTPUT);
  while (!Serial) delay(10);
}

void loop() {
    if (Serial.available()>0)
    {
      int c=Serial.read();
      Serial.print("I received: ");
      Serial.println(c,DEC);
      if(c==126){
        tone(buzzer,440);
        digitalWrite(led,HIGH);
        Serial.println("Sound Alarm!!");                              
        delay(250);
      }
      else{
        noTone(buzzer);
        digitalWrite(led,LOW);
        delay(250);
      }
    }
  }
