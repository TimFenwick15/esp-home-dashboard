#include "credentials.h"
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#define NEOPIXEL_PIN (14)
#define NUMBER_OF_LEDS (30)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_OF_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

uint32_t green = 0x00FF00;
uint32_t red = 0xFF0000;
uint32_t purple = 0x800080;
uint32_t pink = 0xFC0FC0;
uint32_t black = 0x000000;
uint32_t white = 0xFFFFFF;

//const char* ssid     = "Colress4Zero";
//const char* password = "lorddonald";
WiFiServer server(80);
String header;

void setup() {
  //Serial.begin(115200);
  //Serial.print("Connecting to ");
  //Serial.println(ssid);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.println("WiFi connected.");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients
  uint32_t colour;
  bool setColour = false;
  
  if (client) {                             // If a new client connects,
    //Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
          
            if (header.indexOf("GET /green") >= 0) {
              setColour = true;
              colour = green;
            } else if (header.indexOf("GET /red") >= 0) {
              setColour = true;
              colour = red;
            } else if (header.indexOf("GET /purple") >= 0) {
              setColour = true;
              colour = purple;
            } else if (header.indexOf("GET /pink") >= 0) {
              setColour = true;
              colour = pink;
            } else if (header.indexOf("GET /white") >= 0) {
              setColour = true;
              colour = white;
            } else if (header.indexOf("GET /black") >= 0) {
              setColour = true;
              colour = black;
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            // CSS
            client.println("<style>html { font-family: Helvetica; display: inline-block; height: 100%; text-align: center;}body{height: 100%; margin: 0; padding: 0;}");
            client.println(".sixthheight {height: calc(100% / 6); margin: 0 0}");
            client.println(".redbutton { background-color: #FF0000; width: 100%; height: 100%; border: none; color: white; padding: 0px 0px;}");
            client.println(".greenbutton { background-color: #00FF00; width: 100%; height: 100%; border: none; color: white; padding: 0px 0px;}");
            client.println(".purplebutton { background-color: #551A8B; width: 100%; height: 100%; border: none; color: white; padding: 0px 0px;}");
            client.println(".pinkbutton { background-color: #FC0FC0; width: 100%; height: 100%; border: none; color: white; padding: 0px 0px;}");
            client.println(".whitebutton { background-color: #FFFFFF; width: 100%; height: 100%; border: none; color: white; padding: 0px 0px;}");
            client.println(".blackbutton { background-color: #000000; width: 100%; height: 100%; border: none; color: white; padding: 0px 0px;}");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}</style></head><body>");
            
            client.println("<p class=\"sixthheight\"><a href=\"/red\"><button class=\"redbutton\"></button></a></p>");
            client.println("<p class=\"sixthheight\"><a href=\"/green\"><button class=\"greenbutton\"></button></a></p>");    
            client.println("<p class=\"sixthheight\"><a href=\"/purple\"><button class=\"purplebutton\"></button></a></p>");
            client.println("<p class=\"sixthheight\"><a href=\"/pink\"><button class=\"pinkbutton\"></button></a></p>");
            client.println("<p class=\"sixthheight\"><a href=\"/white\"><button class=\"whitebutton\"></button></a></p>");
            client.println("<p class=\"sixthheight\"><a href=\"/black\"><button class=\"blackbutton\"></button></a></p>");
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    //Serial.println("Client disconnected.");
    //Serial.println("");
  }
  if (setColour) {
    strip.begin();
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, colour);
    }
    strip.show();
  }
}

