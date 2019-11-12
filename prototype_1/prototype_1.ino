#include <ESP8266WiFi.h> 
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <SD.h>
#include <BlynkSimpleEsp8266.h> 
#define BLYNK_PRINT Serial
//Line
void Line_Notify(String message);
#define LINE_TOKEN "03KVtej4HJ3nIuqUxeZnVYaauSmq2RztjgGqMQxjiTc"
//#define LINE_TOKEN "JALzghMvg1VIJrVFO1XXqnEh0pxrRcYtGfIFH8EVLmR"
String message = "%20!!%E0%B8%AD%E0%B8%B1%E0%B8%99%E0%B8%95%E0%B8%A3%E0%B8%B2%E0%B8%A2!!%0A%E0%B8%A1%E0%B8%B5%E0%B8%9C%E0%B8%B9%E0%B9%89%E0%B8%9A%E0%B8%B8%E0%B8%81%E0%B8%A3%E0%B8%B8%E0%B8%81%E0%B9%80%E0%B8%82%E0%B9%89%E0%B8%B2%E0%B8%A1%E0%B8%B2%E0%B9%83%E0%B8%99%E0%B8%9A%E0%B9%89%E0%B8%B2%E0%B8%99%0A"; 

//sensor
int pirPin1 = D0;
int pirPin2 = D3;
int reedPin1 = D4;
int reedPin2 = D8;
#define button D1

//WiFi
const char ssid[] = "sMARTz";
const char password[] = "patr8958";

//Timelib
static const char ntpServerName[] = "us.pool.ntp.org";
const int timeZone = 7;
WiFiUDP Udp;
unsigned int localPort = 8888;
time_t getNtpTime();
String digitalClockDisplay();
String printDigits(int digits);
void sendNTPpacket(IPAddress &address);

//Blynk
char auth[] = "e03fe6797bb74c9dad26f23f32f7484a";

//Sdcard
const int chipSelect = 4;
File file;
void writeFile(File myFile,String nameDotTXT,String writingWord);
//void openFile(File myFile,String nameDotTXT);

int pirState1 = 0;
int pirState2 = 0;
int reedState1 = 1;
int reedState2 = 1;
int pirMode1;
int pirMode2;
int homemode;
int vbuttonState = 0;
BLYNK_WRITE(V3){
  pirMode1 = param.asInt();
}
BLYNK_WRITE(V5){
  pirMode2 = param.asInt();
}
BLYNK_WRITE(V8){
  homemode = param.asInt();
}
BLYNK_WRITE(V9){
  vbuttonState = param.asInt();
}
BLYNK_CONNECTED(){
  Blynk.syncAll();
}

WidgetLCD lcd(V1);
WidgetLED led1(V2);
WidgetLED led2(V4);
WidgetLED led3(V6);
WidgetLED led4(V7);

void setup() {
  // put your setup code here, to run once:
  pinMode(button,INPUT);
  //pinMode(led,OUTPUT);
  pinMode(pirPin1,INPUT);
  pinMode(pirPin2,INPUT);
  pinMode(SS, OUTPUT);
  pinMode(reedPin1,INPUT);
  pinMode(reedPin2,INPUT);
  //pinMode(buzzer,OUTPUT);
  Serial.begin(115200); 
  Blynk.begin(auth,ssid,password);  
  Blynk.virtualWrite(V3,HIGH);
  
  Blynk.virtualWrite(V5,HIGH);
  WiFi.begin(ssid, password); 
  //Serial.print("connecting");
  while(WiFi.status()!= WL_CONNECTED) 
  {     
    delay(10);     
    //Serial.print(".");   
  }
  while (!Serial) delay(10); 
  /*Serial.println("");   
  Serial.println("WiFi connected");     
  Serial.println("IP address: ");   
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");*/
  Udp.begin(localPort);
  /*Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");*/
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  if (!SD.begin(chipSelect)) {
    //Serial.println("initialization failed!");
    return;
  }
  //Serial.println("initialization done.");
}

time_t prevDisplay = 0; // when the digital clock was displayed

void loop() {
  // put your main code here, to run repeatedly:
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
    }
  }
  Blynk.run(); 
  //Serial.print("homemode: ");
  //Serial.println(homemode);
  lcd.print(5,0,"Ready!!");
  lcd.print(1,1,"Status : SAFE");
  pirState1 = digitalRead(pirPin1);
  pirState2 = digitalRead(pirPin2);
  reedState1 = digitalRead(reedPin1);
  reedState2 = digitalRead(reedPin2);
  led1.off();
  led2.off();
  led3.off();
  led4.off();
  if(homemode == 1){
    if((pirState1==1)&&(pirMode1==1 && pirMode2==1)){
      lcd.clear();
      led1.on();
      writeFile(file,"Date.txt",digitalClockDisplay());
      homemode = alarm();
      lcd.clear();
    } 
    else if((pirState2==1)&&(pirMode1==1 && pirMode2==1)){
      lcd.clear();
      led2.on();
      writeFile(file,"Date.txt",digitalClockDisplay());
      homemode = alarm();
      lcd.clear();
    } 
    else if(pirState2==1 && pirMode1==0){
      lcd.clear();
      led2.on();
      writeFile(file,"Date.txt",digitalClockDisplay());
      homemode = alarm();   
      lcd.clear();                  
    }
    else if(pirState1==1 && pirMode2==0){
      lcd.clear();
      led1.on();
      writeFile(file,"Date.txt",digitalClockDisplay());
      homemode = alarm(); 
      lcd.clear(); 
    }
    else if(reedState1 ==1){
      lcd.clear();
      led3.on();
      writeFile(file,"Date.txt",digitalClockDisplay());
      homemode = alarm();
      lcd.clear();
    }
    else if(reedState2 ==0){
      lcd.clear();
      led4.on();
      writeFile(file,"Date.txt",digitalClockDisplay());
      homemode = alarm();
      lcd.clear();
    }
  }
  /*Serial.print("pirState1 : ");
  Serial.println(pirState1);
  Serial.print("pirState2 : ");
  Serial.println(pirState2);
  Serial.print("reedState1 : ");
  Serial.println(reedState1);
  Serial.print("reedState2 : ");
  Serial.println(reedState2);*/
}

int alarm(){
  int buttonState;
  int homemode=1;
  int lineState=0;
  while(homemode==1)
  { 
    Serial.write(126); 
    lcd.print(5,0,"Intruder!!");
    lcd.print(1,1,"Status : DANGER");
    //Serial.println(buttonState);
    if(lineState==0){
      Line_Notify(message); 
      lineState=1;
    }
    buttonState=digitalRead(button);
    if(buttonState==HIGH || vbuttonState==1){
      homemode=0;
      Blynk.virtualWrite(V8,LOW);
      Serial.write(64);
    }
  }
  return homemode;
}

void Line_Notify(String message){   
  WiFiClientSecure client; 
  if (!client.connect("notify-api.line.me", 443)){     
    //Serial.println("connection failed");     
    return;      
  } 

  String req = "";   
  req += "POST /api/notify HTTP/1.1\r\n";  
  req += "Host: notify-api.line.me\r\n";   
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";   
  req += "Cache-Control: no-cache\r\n";   
  req += "User-Agent: ESP8266\r\n"; 
  req += "Content-Type: application/x-www-form-urlencoded\r\n";   
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";   
  req += "\r\n";   
  req += "message=" + message;   
  // Serial.println(req);   
  client.print(req);        
  delay(20); 
  // Serial.println("-------------");   
  while(client.connected()){     
    String line = client.readStringUntil('\n');     
    if (line == "\r"){       
      break;     
    }     
    //Serial.println(line);   
  }   
  // Serial.println("-------------"); } 
}

//Timelib
String digitalClockDisplay()
{
  String datetime = "Date of the incident : Time :";
  // digital clock display of the time
  //Serial.print(hour());
  datetime += hour();
  datetime += ":";
  datetime += printDigits(minute());
  datetime += ":";
  datetime += printDigits(second());
  //Serial.print(" ");
  datetime += " Date: ";
  //Serial.print(day());
  datetime += day();
  //Serial.print(".");
  
  datetime += ".";
  //Serial.print(month());
  datetime += month();
  //Serial.print(".");
  datetime += ".";
  //Serial.print(year());
  datetime += year();
  //Serial.println();
  return datetime;
}

String printDigits(int digits)
{
  String datetime;
  // utility for digital clock display: prints preceding colon and leading 0
  if (digits < 10){
    datetime += "0";
  }
  datetime += digits;
  return datetime;
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  //Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  //Serial.print(ntpServerName);
  //Serial.print(": ");
  //Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      //Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  //Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

//Sdcard
void writeFile(File myFile,String nameDotTXT,String writingWord){
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open(nameDotTXT, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    //Serial.print("Writing to text File...");
    myFile.println(writingWord);
    // close the file:
    myFile.close();
    //Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    //Serial.println("error opening ");
  }
}

/*void openFile(File myFile,String nameDotTXT){
  myFile = SD.open(nameDotTXT);
  if (myFile) {
    Serial.println(nameDotTXT+" : ");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening ");
  }  
}*/

