#include "credentials.h"
#include "html.h"
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <math.h>

/*
 * Temperature Defines
 */
#define ZERO_CELCIUS (273.15f)
#define R_Balance (9850.0f) // 10K +- 5% resistor
#define T0 (ZERO_CELCIUS + 25)
#define B (4300.0f) // I didn't have a datasheet for my thermistor, this value is in the correct region
#define R0 (53000.0f) // Resistance can be +- 10%, this value worked for my thermistor
#define TEN_BIT_MAX (1023.0f)
#define V_SOURCE (3.3f)

#define THERMISTOR_ADC_PIN (A0)
#define THERMISTOR_SOURCE_PIN (12)
#define NEOPIXEL_PIN (14)
#define NUMBER_OF_LEDS (30)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_OF_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

int temperature(void);

uint32_t green = 0x00FF00;
uint32_t red = 0xFF0000;
uint32_t purple = 0x800080;
uint32_t pink = 0xFC0FC0;
uint32_t black = 0x000000;
uint32_t white = 0xFFFFFF;

WiFiServer server(80);
String header;
sensorData data;

void setup() {
  //Serial.begin(115200);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  server.begin();
  //Serial.println(WiFi.localIP());
  strip.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients
  uint32_t colour;
  bool setColour = false;
  
  if (client) {                             // If a new client connects,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);
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
            
            data.temperature = temperature();
            
            if (header.indexOf("GET /temperature") >= 0) {
              client.println(data.temperature);
            }
            else {
              client.println(makeHTML(data));
            }
            
            client.println(); // The HTTP response ends with another blank line
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
  }
  if (setColour) {
    strip.begin();
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, colour);
    }
    strip.show();
  } 
}

int temperature(void) {
  float R,T,L;
  R = R_Balance * (V_SOURCE * TEN_BIT_MAX / float(analogRead(THERMISTOR_ADC_PIN)) - 1.0f);
  T = 1.0f / (1.0f / T0 + log(R / R0) / B);
  T = T - ZERO_CELCIUS;
  return (int) T;
}

