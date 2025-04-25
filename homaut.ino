#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include<Servo.h>

//SSID and Password to your ESP Access Point
const char* ssid = "Roboduino";
const char* password = "Roboduino";

#define Light D5     //   GPIO4(D2)
#define Fan D3         // GPIO0(D3)
#define Door D7     //  GPIO2(D4)
/*#define IN_3  2    //          GPIO12(D6)
#define IN_4  0      //       GPIO13(D7)
#define ENB   12      GPIO15(D8)*/


Servo myServo;
String command;
ESP8266WebServer server(80);

void setup() {

  Serial.begin(115200);
  pinMode(Light, OUTPUT);
  pinMode(Fan, OUTPUT);
  myServo.attach(D7);
//  myServo.write(i);
  // Connecting WiFi

  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Starting WEB-server
  server.on ( "/", HTTP_handleRoot );
  server.onNotFound ( HTTP_handleRoot );
  server.begin();
}

void loop() {
  server.handleClient();

  command = server.arg("State");
       if (command == "L") lighton();
  else if (command == "R") lightoff();
  else if (command == "F") fanon();
  else if (command == "Q") fanoff();
  else if (command == "D") dooropen();
  else if (command == "C") doorclose();

}

void HTTP_handleRoot(void) {

  if ( server.hasArg("State") ) {
    Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(100);
}

void lighton(){
  digitalWrite(Light,HIGH);
}

void lightoff(){
  digitalWrite(Light,LOW);
}

void fanon(){
  analogWrite(Fan,255);
}

void fanoff() {
  analogWrite(Fan,0);
}

void dooropen(){
  for(int i = 95; i<=175; i++){
      myServo.write(i);
      delay(100);
      }
    myServo.write(179);  
    Serial.println("Door is open!");
}

void doorclose(){
  for(int i = 175; i>=95; i--){
      myServo.write(i);
      delay(100);
      }
      myServo.write(95);
    Serial.println("Door is closed!");
}
