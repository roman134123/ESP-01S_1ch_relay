/* 
  ###################################################################################
  ## Скетч к проекту одноканального реле на модуле ESP01S и платы расширения       ##
  ## Группа VK для заказа товаро: https://vk.com/retech_msc                        ##
  ## Источник: https://www.instructables.com/ESP0101S-RELAY-MODULE-TUTORIAL/       ##
  ###################################################################################
*/

#include <ESP8266WiFi.h>                                              // Подключение библиотеки ESP8266WiFi

const char* ssid = "Имя вашей Wi-Fi сети";                            // Имя вашей Wi-Fi сети
const char* password = "Пароль вашей Wi-Fi сети";                     // Пароль вашей Wi-Fi сети
#define RELAY 0                                                       // Номер пина ESP01, к которому подключено реле
WiFiServer server(80);                                                // Порт WEB-сервера
 
void setup() 
{
  Serial.begin(115200);                                               // Указывается сокрость передачи данных 
 
  pinMode(RELAY,OUTPUT);
  digitalWrite(RELAY, LOW);
 
  
  Serial.println();
  Serial.println();
  Serial.print("Подключение к Wi-Fi ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);                                         // Подключение к Wi-Fi
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi подключен");
 
  server.begin();
  Serial.println("Сервер запущен");
 
  
  Serial.print("Исопльзуйте этот IP-адрес для подключения: ");
  Serial.print(" ");                                                  // Вывод IP-адреса
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() 
{
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }
 
  Serial.println("new client");
  while(!client.available())
  {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  int value = LOW;
  if (request.indexOf("/RELAY=ON") != -1)  
  {
    Serial.println("RELAY=ON");
    digitalWrite(RELAY,LOW);
    value = LOW;
  }
  if (request.indexOf("/RELAY=OFF") != -1)  
  {
    Serial.println("RELAY=OFF");
    digitalWrite(RELAY,HIGH);
    value = HIGH;
  }
  
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  this is a must
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><title>ESP8266 RELAY Control</title></head>");
  client.print("Relay is now: ");
 
  if(value == HIGH) 
  {
    client.print("OFF");
  } 
  else 
  {
    client.print("ON");
  }
  client.println("<br><br>");
  client.println("Turn <a href=\"/RELAY=OFF\">OFF</a> RELAY<br>");
  client.println("Turn <a href=\"/RELAY=ON\">ON</a> RELAY<br>");
    client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
