// #include <Arduino.h>
// #include <WiFi.h>

#include <wifishen.h> 

WiFiServer server(80);

void wifi_setup(const char* ssid, const char* password) {
  delay(10);
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
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void wifi_loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {

          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            
            // client.print("<a href=\"/D\">default</a><br>");
            // client.print("<a href=\"/P\">start petting</a><br>");
            // client.print("<a href=\"/S\">ask to search for missing sock</a><br>");

            client.print("<p>Nerdville</p><br>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        // if (currentLine.endsWith("GET /D")) {
        //   currentAnim = 0;
        //   frameIndex = 0;
        // } else if (currentLine.endsWith("GET /P")) {
        //   currentAnim = 1;
        //   frameIndex = 0;
        // } else if (currentLine.endsWith("GET /S")) {
        //   currentAnim = 2;
        //   frameIndex = 0;
        // }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}