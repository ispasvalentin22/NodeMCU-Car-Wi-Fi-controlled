// Definim pinul GPIO14(D5) pentru activarea și accelerarea 
// motorului din dreapta ca fiind ENA
#define ENA   14
#define ENB   12    
#define IN_1  15   
// Definim IN_2 (motorul din dreapta direcția față) ca pinul GPIO13(D7)
#define IN_2  13  
#define IN_3  2  
#define IN_4  0   

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

String command;   // String pentru stocarea comenzii primite de la aplicație
int speedCar = 800;  // Viteza implicită a motoarelor
int speed_Coeff = 3;
const char* ssid = "NodeMCU Car"; 
ESP8266WebServer server(80);  // Crearea obiectului server pentru a gestiona
                              // cererile HTTP primite din aplicație
void setup() {
 
 pinMode(ENA, OUTPUT); // Setează pinul ENA pentru ieșire
 pinMode(ENB, OUTPUT);  
 pinMode(IN_1, OUTPUT);
 pinMode(IN_2, OUTPUT); // Setează pinul IN_2 pentru ieșire
 pinMode(IN_3, OUTPUT);
 pinMode(IN_4, OUTPUT); 

 // Începe comunicarea cu calculatorul pentru a transmite mesaje
  Serial.begin(115200);  
  
// Configurarea rețelei WiFi
  WiFi.mode(WIFI_AP);
// Numele rețelei WiFi a microcontrolerului ia valoarea stocată în stringul ssid
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP); // Afișează adresa IP a rețelei WiFi
 
 // Pornirea unui server WEB 
 server.on ( "/", HTTP_handleRoot);
 server.onNotFound ( HTTP_handleRoot );
 server.begin();    
}
void goAhead(){ // Funcția care mișcă mașina în față
 digitalWrite(IN_1, LOW);
 // Activează pinul IN_2 (Motorul din dreapta pornește în față)
 digitalWrite(IN_2, HIGH); 
 analogWrite(ENA, speedCar);
 digitalWrite(IN_3, LOW);
 // Activează pinul IN_4 (Motorul din stânga pornește în față)
 digitalWrite(IN_4, HIGH);
// Pinul ENB setează viteza motoarelor la valoarea stocată în speedCar
 analogWrite(ENB, speedCar); 
  }
void goBack(){ // Funcția care mișcă mașina în spate
 digitalWrite(IN_1, HIGH); // Motorul din dreapta pornește în spate
 digitalWrite(IN_2, LOW);
 analogWrite(ENA, speedCar);
 digitalWrite(IN_3, HIGH); // Motorul din stânga pornește în spate
 digitalWrite(IN_4, LOW);
 // Pinul ENB setează viteza motoarelor la valoarea stocată în speedCar
 analogWrite(ENB, speedCar);
  }
void goRight(){ // Funcția care mișcă mașina în dreapta
 digitalWrite(IN_1, HIGH); // Motorul din dreapta pornește în spate 
 digitalWrite(IN_2, LOW);
 analogWrite(ENA, speedCar);
 digitalWrite(IN_3, LOW);
 digitalWrite(IN_4, HIGH); // Motorul din stânga pornește în față
 // Pinul ENB setează viteza motoarelor la valoarea stocată în speedCar
 analogWrite(ENB, speedCar);
 }
void goLeft(){ // Funcția care mișcă mașina în stânga
 digitalWrite(IN_1, LOW);
 digitalWrite(IN_2, HIGH); // Motorul din dreapta pornește în față
 analogWrite(ENA, speedCar);
 digitalWrite(IN_3, HIGH); // Motorul din stânga pornește în spate
 digitalWrite(IN_4, LOW);
 // Pinul ENB setează viteza motoarelor la valoarea stocată în speedCar
 analogWrite(ENB, speedCar);
 }
void goAheadRight(){ // Funcția care mișcă mașina în față-dreapta
 digitalWrite(IN_1, LOW);
 digitalWrite(IN_2, HIGH); // Motorul din dreapta pornește în față
 analogWrite(ENA, speedCar/speed_Coeff);
 digitalWrite(IN_3, LOW);
 digitalWrite(IN_4, HIGH);
 // Pinul ENB setează viteza motoarelor la valoarea stocată în speedCar
 analogWrite(ENB, speedCar);
 }
void goAheadLeft(){ // Funcția care mișcă mașina în față-stânga
 digitalWrite(IN_1, LOW);
 digitalWrite(IN_2, HIGH); // Motorul din dreapta pornește în față
 analogWrite(ENA, speedCar);
 digitalWrite(IN_3, LOW);
 digitalWrite(IN_4, HIGH);
 // Pinul ENB setează viteza motoarelor la valoarea din speedCar împărțită la speed_Coeff
 analogWrite(ENB, speedCar/speed_Coeff);
  }
void goBackRight(){ // Funcția care mișcă mașina în spate-dreapta
 digitalWrite(IN_1, HIGH); // Motorul din dreapta pornește în spate
 digitalWrite(IN_2, LOW);
 analogWrite(ENA, speedCar/speed_Coeff);
 digitalWrite(IN_3, HIGH); // Motorul din stânga pornește în spate
 digitalWrite(IN_4, LOW);
// Pinul ENB setează viteza motoarelor la valoarea stocată în speedCar
 analogWrite(ENB, speedCar);
  }
void goBackLeft(){ // Funcția care mișcă mașina în spate-stânga
 digitalWrite(IN_1, HIGH); // Motorul din dreapta pornește în spate
 digitalWrite(IN_2, LOW);
 analogWrite(ENA, speedCar);
 digitalWrite(IN_3, HIGH); // Motorul din stânga pornește în spate
 digitalWrite(IN_4, LOW);
// Pinul ENB setează viteza motoarelor la valoarea din speedCar împărțită la speed_Coeff
 analogWrite(ENB, speedCar/speed_Coeff);
  }
void stopRobot(){ // Funcția care oprește mașina pe loc
 digitalWrite(IN_1, LOW); 
 digitalWrite(IN_2, LOW); // Motorul din dreapta se oprește
 analogWrite(ENA, speedCar);
 digitalWrite(IN_3, LOW);
 digitalWrite(IN_4, LOW); // Motorul din stânga se oprește
// Pinul ENB setează viteza motoarelor la valoarea stocată în speedCar
 analogWrite(ENB, speedCar);
 }
void loop() {
 server.handleClient(); // Așteaptă o comandă de la aplicație
 command = server.arg("State");  // Se stochează comanda primită din aplicație
 // Execută funcția goAhead() când se primește comanda "F" din aplicație
 if (command == "F") goAhead();  
 else if (command == "B") goBack();
 else if (command == "L") goLeft();
 // Execută funcția goRight() când se primește comanda "R" din aplicație
 else if (command == "R") goRight(); 
 else if (command == "I") goAheadRight();
 else if (command == "G") goAheadLeft();
 // Execută funcția goBackRight() când se primește comanda "J" din aplicație
 else if (command == "J") goBackRight();  
 else if (command == "H") goBackLeft();
 // Viteza motoarelor devine 400 atunci când se primește comanda "O" din aplicație
 else if (command == "0") speedCar = 400;  
 else if (command == "1") speedCar = 470;
 else if (command == "2") speedCar = 540;
 // Viteza motoarelor devine 610 atunci când se primește comanda "3" din aplicație
 else if (command == "3") speedCar = 610;  
 else if (command == "4") speedCar = 680;
 else if (command == "5") speedCar = 750;
 // Viteza motoarelor devine 820 atunci când se primește comanda "6" din aplicație
 else if (command == "6") speedCar = 820;  
 else if (command == "7") speedCar = 890;
 else if (command == "8") speedCar = 960;
 // Viteza motoarelor devine 1023 atunci când se primește comanda "9" din aplicație
 else if (command == "9") speedCar = 1023; 
 // Execută funcția stopRobot() atunci când se primește comanda "S" din aplicație
 else if (command == "S") stopRobot();  
}
void HTTP_handleRoot(void) { // Funcția care gestionează solicitările HTTP
if( server.hasArg("State") ){
     Serial.println(server.arg("State"));
  }
// Serverul trimite codul HTTP 200 (OK)
  server.send ( 200, "text/html", "" );
  delay(1);
}
