int reedPin1 = 4;
int reedPin2 = 5;
int reedState1 = 0;
int reedState2 = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(reedPin1,INPUT);
  pinMode(reedPin2,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  reedState1 = digitalRead(reedPin1);
  reedState2 = digitalRead(reedPin2);
  Serial.print("reedState1 : ");
  Serial.println(reedState1);
  Serial.print("reedState2 : ");
  Serial.println(reedState2);
}
