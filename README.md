A webpage served by an Adafruit ESP8266 Feather Huzzah. Used to set colours on a Neopixel strip, and report temperature from a 503 thermistor.

## Software requirements
- Tested on Windows only
- nodejs
- Arduino

## To build
The webpage shown by the ESP is written in index.html.

For this to be uploaded to the ESP, it must be built into a header file, esp/html.h.

This can be done with "npm run build"

This will also run "arduino_debug" to verify that the Arduino code compiles. This requires your Arduino install directory to be added to your path environment variable.

## credentials.h
esp/credentials.h must exist for the Arduino code to compile. A credentials.example.h is included. Enter your WiFi name and password in this file, leaving the "" in place, and rename this file to credentials.h.

## Sources
The esp.h webserver code is based on https://randomnerdtutorials.com/esp8266-web-server/ :heart:

Information on using thermistors was taken from https://arduino.stackexchange.com/questions/6875/how-to-use-503-thermistors-for-temperature :heart:

An example 503 thermistor data sheet can be found here: https://www.jameco.com/Jameco/Products/ProdDS/207002.pdf

## Todo
- Wouldn't ideally have 0-1V across the thermistor constantly. Supply 3V3 on read only?
- Step down 5V Neopixel supply to 3V3
