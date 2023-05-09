#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

int IN1 = 22;
int IN2 = 19;
int IN3 = 21;
int IN4 = 23;

bool LED1status = LOW;
bool LED2status = LOW;
bool LED3status = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.on("/led3on", handle_led3on);
  server.on("/led3off", handle_led3off);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  if(LED2status && !LED3status) { // LED2 is on & LED3 is off 
    Serial.println("LED2 ON | LED3 OFF");
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else if (LED2status && LED3status) { // Both on
    Serial.println("LED2 ON | LED3 ON");
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  } else if (!LED2status && LED3status) { // LED2 is off, LED3 is on
    Serial.println("LED2 OFF | LED3 ON");
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else { // both off
    Serial.println("LED2 OFF | LED3 OFF");
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
  }

  if(LED1status) { // LED1 is on
    Serial.println("LED1 ON");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  } else { // LED2 is off
    Serial.println("LED1 OFF");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
  }

  delay(1000);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  LED3status = LOW;
  Serial.println("GPIO22 Status: OFF | GPIO15 Status: OFF | GPIO23 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status, LED3status)); 
}

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO22 Status: ON");
  server.send(200, "text/html", SendHTML(true,LED2status, LED3status)); 
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO22 Status: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status, LED3status)); 
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO15 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,true, LED3status)); 
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO15 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false, LED3status)); 
}

void handle_led3on() {
  LED3status = HIGH;
  Serial.println("GPIO23 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,LED2status, true)); 
}

void handle_led3off() {
  LED3status = LOW;
  Serial.println("GPIO23 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status, false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat, uint8_t led3stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #2c3e50;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #f8ec07;}\n";
  ptr +=".button-on:active {background-color: #f8ec07;}\n";
  ptr +=".button-off {background-color: #2c3e50;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "a {-webkit-transform: rotate(90deg);}\n";
  ptr += "ul { display: -webkit-flex; margin: 0px auto;max-width: 400px;list-style-type: none;}\n";
  ptr += "li {  -webkit-flex: 1;padding: 0px;width: 30px;height: 30px;position: relative;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Color Change Control</h1>\n";
  ptr +="<h3>Design: Battery</h3>\n";
  
  ptr += "<ul>\n";
   if(led1stat)
  {ptr +="<li><a class=\"button button-on\" href=\"/led1off\">OFF</a></li>\n";}
  else
  {ptr +="<li><a class=\"button button-off\" href=\"/led1on\">ON</a></li>\n";}

  if(led2stat)
  {ptr +="<li><a class=\"button button-on\" href=\"/led2off\">OFF</a></li>\n";}
  else
  {ptr +="<li><a class=\"button button-off\" href=\"/led2on\">ON</a></li>\n";}

  if(led3stat)
  {ptr +="<li><a class=\"button button-on\" href=\"/led3off\">OFF</a></li>\n";}
  else
  {ptr +="<li><a class=\"button button-off\" href=\"/led3on\">ON</a></li>\n";}
  ptr += "</ul>\n";

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}