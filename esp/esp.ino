/**
 * Includes
 */

#include "credentials.h"
#include "html.h"
#include "weather.h"
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <math.h>

/**
 * Macros
 */

#define SERIAL_LOGGING
#define COUNTER_MAX (600) // 10 minutes of 100ms loops

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

#define GREEN (0x00FF00)
#define RED (0xFF0000)
#define PURPLE (0x800080)
#define PINK (0xFC0FC0)
#define BLACK (0x000000)
#define WHITE (0xFFFFFF)
#define YELLOW (0xFFFF00)
#define ORANGE (0xFFA500)
#define BLUE (0x0000FF)

/**
 * Function prototypes
 */

static void temperature(sensorData* data);
static void weather(sensorData* data);

/**
 * Local varaibles
 */

static Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_OF_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
static WiFiServer server(80);
static sensorData data;
static String header;
static uint16_t counter = COUNTER_MAX; // Initialise to counter max so our network request runs on start up

/**
 * Function definitions
 */

void setup() {
#ifdef SERIAL_LOGGING
  Serial.begin(115200);
#endif
  WiFi.mode(WIFI_STA); // Otherwise the ESP will show as an open access point
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  server.begin();
  strip.begin();
#ifdef SERIAL_LOGGING
  Serial.println(WiFi.localIP());
#endif
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
#ifdef SERIAL_LOGGING
        Serial.write(c);
#endif 
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
          
            if (header.indexOf("GET /green") >= 0) {
              setColour = true;
              colour = GREEN;
            } else if (header.indexOf("GET /red") >= 0) {
              setColour = true;
              colour = RED;
            } else if (header.indexOf("GET /purple") >= 0) {
              setColour = true;
              colour = PURPLE;
            } else if (header.indexOf("GET /pink") >= 0) {
              setColour = true;
              colour = PINK;
            } else if (header.indexOf("GET /white") >= 0) {
              setColour = true;
              colour = WHITE;
            } else if (header.indexOf("GET /black") >= 0) {
              setColour = true;
              colour = BLACK;
            } else if (header.indexOf("GET /yellow") >= 0) {
              setColour = true;
              colour = YELLOW;
            } else if (header.indexOf("GET /orange") >= 0) {
              setColour = true;
              colour = ORANGE;
            } else if (header.indexOf("GET /blue") >= 0) {
              setColour = true;
              colour = BLUE;
            }
            
            if (header.indexOf("GET /temperature") >= 0) {
              client.println(data.temperature);
            }
            else {
              makeHTML(client, data);
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

  counter++;
  if (counter > COUNTER_MAX) {
    counter = 0;
    temperature(&data);
    weather(&data);
  }
  delay(100);
}

static void temperature(sensorData* data) {
  float R, T, L;
  R = R_Balance * (V_SOURCE * TEN_BIT_MAX / float(analogRead(THERMISTOR_ADC_PIN)) - 1.0f);
  T = 1.0f / (1.0f / T0 + log(R / R0) / B);
  T = T - ZERO_CELCIUS;
  data->temperature = (int) T;
}

static void weather(sensorData* data) {
  uint8_t weatherCount = 8;
  uint8_t i;
  uint8_t result;
  weatherType response = noWeather;
  HTTPClient http;
  int httpResponseCode;
  String httpResponseBody;
  String summarySubstring;
  String summary;
  String temperatureSubstring;
  int temperature;
  http.begin(WEATHER_API_URL);
  httpResponseCode = http.GET();
  if (httpResponseCode == 200) {
    httpResponseBody = http.getString();
    summarySubstring = httpResponseBody.substring(httpResponseBody.indexOf("description\":\"") + 14);
    summary = summarySubstring.substring(0, summarySubstring.indexOf("\""));
    summary.toLowerCase();

    // Get the weather type
    if (summary.indexOf("sun") != -1) {
      data->weather = weatherText[sunny];
    }
    if (
      (summary.indexOf("cloud") != -1) ||
      (summary.indexOf("clear") != -1) ||
      (summary.indexOf("overcast") != -1) ||
      (summary.indexOf("fog") != -1) ||
      (summary.indexOf("mist") != -1) ||
      (summary.indexOf("haze") != -1)) {
      data->weather = weatherText[cloudy];
    }
    if (
      (summary.indexOf("rain") != -1) ||
      (summary.indexOf("drizzle") != -1)) {
      data->weather = weatherText[rainy];
    }
    if (summary.indexOf("snow") != -1) {
      data->weather = weatherText[snowy];
    }
    Serial.println(data->weather);
    // Get the temperature
    temperatureSubstring = httpResponseBody.substring(httpResponseBody.indexOf("temp\":") + 6);
    temperature = temperatureSubstring.substring(0, temperatureSubstring.indexOf(".")).toInt(); // Use . as delim to drop decimal points
    temperature = temperature - (int)ZERO_CELCIUS;
    data->weatherTemperature = temperature;
  }
  http.end();
}
