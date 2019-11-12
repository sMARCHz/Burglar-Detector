int pir = 5;
int led = 3;
int pirState=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pir,INPUT);
  pinMode(led,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  pirState = digitalRead(pir);
  Serial.print("pirState : ");
  Serial.println(pirState);
  if(pirState == HIGH)
  {
    digitalWrite(led,HIGH);
    delay(250);
  }
  else{
    digitalWrite(led,LOW);
  }
}
