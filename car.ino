//http://arduino.esp8266.com/stable/package_esp8266com_index.json
#include <ESP8266WiFi.h>
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
const char* ssid = "********";
const char* password = "********";
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
WiFiServer server(80);
unsigned long int timer, timer2, timer3;
int  rain = 1024, val0 = 0, val = 1, val1 = 1, val2 = 1, val3 = 1, val4 = 1, val5 = 1, val6 = 1, val7 = 1, val8 = 1, val9 = 0;
//...............................................
void setup() {
  Serial.begin(115200); //Default Baud Rate for NodeMCU
  delay(10);
  pinMode(0, OUTPUT);
  digitalWrite(0, 1);
  pinMode(16, INPUT);
  digitalWrite(16, 0);
  pinMode(2, OUTPUT);
  digitalWrite(2, 1);
  pinMode(3, OUTPUT);
  digitalWrite(3, 1);
  pinMode(5, OUTPUT);
  digitalWrite(5, 1);
  pinMode(12, OUTPUT);
  digitalWrite(12, 1);
  pinMode(13, OUTPUT);
  digitalWrite(13, 1);
  pinMode(14, OUTPUT);
  digitalWrite(14, 1);
  pinMode(15, OUTPUT);
  digitalWrite(15, 1);
  pinMode(4, INPUT);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  for (int i = 0; i < 10; i++) {
    digitalWrite(2, 0);
    delay(200);
    digitalWrite(2, 1);
    delay(200);
  }
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}
//...............................................
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    if (val9 == 1) { //Auto
      if ((digitalRead(4) == HIGH) && (digitalRead(16) == HIGH)) {
        digitalWrite(14, LOW);
        digitalWrite(5, LOW);
      }
      else if ((digitalRead(4) == HIGH) && (digitalRead(16) == LOW)) {
        digitalWrite(14, HIGH);
        digitalWrite(5, LOW);
      }
      else if ((digitalRead(4) == LOW) && (digitalRead(16) == LOW)) {
        digitalWrite(14, HIGH);
        digitalWrite(5, HIGH);
      }
      while ((millis() - timer2) > 800) {            //manual + Low
        rain = analogRead(A0);
        Serial.println(rain);
        if (rain < 700) digitalWrite(3, 0);
        else if (rain > 1000) digitalWrite(3, 1);
        else if ((rain > 700) && (rain < 1000)) {
          while ((millis() - timer3) > 2000) {            //manual + Low
            digitalWrite(3, !digitalRead(3));
            timer3 = millis();
          }
        }
        timer2 = millis();
      }
    }
    if (val8 == 2) {
      while ((millis() - timer) > 2000) {            //manual + Low
        digitalWrite(3, !digitalRead(3));
        timer = millis();
      }
    }
    return;
  }
  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  if (req.indexOf("/gpio/10") != -1)
    val1 = 0;
  else if (req.indexOf("/gpio/11") != -1)
    val1 = 1;
  else if (req.indexOf("/gpio/20") != -1)
    val2 = 0;
  else if (req.indexOf("/gpio/21") != -1)
    val2 = 1;
  else if (req.indexOf("/gpio/30") != -1)
    val3 = 0;
  else if (req.indexOf("/gpio/31") != -1)
    val3 = 1;
  else if (req.indexOf("/gpio/40") != -1)
    val4 = 0;
  else if (req.indexOf("/gpio/41") != -1)
    val4 = 1;
  else if (req.indexOf("/gpio/50") != -1)
    val5 = 0;
  else if (req.indexOf("/gpio/51") != -1)
    val5 = 1;
  else if (req.indexOf("/gpio/60") != -1)
    val6 = 0;
  else if (req.indexOf("/gpio/61") != -1)
    val6 = 1;
  else if (req.indexOf("/gpio/70") != -1)
    val7 = 0;
  else if (req.indexOf("/gpio/71") != -1)
    val7 = 1;
  else if (req.indexOf("/gpio/82") != -1)
    val8 = 0;
  else if (req.indexOf("/gpio/81") != -1)
    val8 = 1;
  else if (req.indexOf("/gpio/83") != -1)
    val8 = 2;
  else if (req.indexOf("/gpio/90") != -1)
    val9 = 0;
  else if (req.indexOf("/gpio/91") != -1)
    val9 = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(15, val1);
  digitalWrite(2, val2);
  digitalWrite(0, val3);
  digitalWrite(13, val4);
  digitalWrite(12, val5);
  digitalWrite(14, val6);
  digitalWrite(5, val7);
  digitalWrite(3, val8);
  Serial.print("auto = ");
  Serial.println(val9);
  client.flush();
}
