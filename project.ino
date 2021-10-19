  #include <ESP8266WiFi.h>
  int sensorPin = A0;
  double alpha = 0.75;
  int period = 100;
  double change = 0;
  double minval = 0;
  double h_val = 0;
  //const char* ssid = "Charcoal is cat_2.4GHz";
  //const char* password = "PpHp0930584886PH=99";
  const char* ssid = "new";
  const char* password = "wdx18000";
  WiFiServer server(80);

void setup(){
  Serial.begin(9600);
  pinMode(D1, OUTPUT); 
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");   
  server.begin();                  
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void Passive_Buzzers(){
  Serial.println("111");
  digitalWrite(D1, HIGH); 
  


}



void loop(){
  //Passive_Buzzers();
  static double oldValue = 0;
  static double oldChange = 0;
  int rawValue = analogRead(sensorPin);
  double value = alpha * oldValue + (1 - alpha) * rawValue;
  if(value < 50){
    Passive_Buzzers();
    }
    else{
      if(h_val < value){
      digitalWrite(D1, HIGH);
      delay(100);
      digitalWrite(D1, LOW);
      delay(1000);
        }
      }
  Serial.println(rawValue);
  //Serial.print(",");
  //Serial.println(value);
  oldValue = value;
  delay(period);

  delay(100);

  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  //Serial.println("new client");
  while (!client.available())
  {
    delay(1);
  }
  String req = client.readStringUntil('\r');
  //Serial.println(req);
  client.flush();
  String web = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  web += "<html>\r\n";   
  web += "<meta http-equiv=\"refresh\" content=\"0\">";
  web += "<body >\r\n";
  web += "<center>\r\n";
  web += "<h1>heartbeat sensor</h1>\r\n";
  web += "<h2>\r\n";
  if (value > 1){     
    web += "Value = "+String(value)+"\r\n";  
    }  
  else{   
    web += "Value 0\r\n";   
    }
  web += "</h2>\r\n";
  web += "</center>\r\n";
  web += "</body>\r\n";  
  web += "</html>\r\n";
  

  client.print(web); 
  h_val = value;
}
