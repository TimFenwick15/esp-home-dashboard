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

credentials.h also includes a weather API URL. The weather API being used is OpenWeatherMap. The url should be:

http://api.openweathermap.org/data/2.5/forecast?q={your location},uk&APPID={your API key}

where {your location} and {your API key} are replaced with your details. More information on this can be found at https://openweathermap.org/api

## Build process
- index.html is minified
- Template text is replaced with a struct member so the ESP can send the complete webpage
- The weather icon is chosen by:
  - making each icon display: none
  - giving it a class name describing what weather type it represents
  - making a display: block class with a template name
  - replace this template name with data.weather so the C code can pick which icon to switch to display: block
  - There's probably a better way to do this...

## Sources
The esp.h webserver code is based on https://randomnerdtutorials.com/esp8266-web-server/ :heart:

Information on using thermistors was taken from https://arduino.stackexchange.com/questions/6875/how-to-use-503-thermistors-for-temperature :heart:

An example 503 thermistor data sheet can be found here: https://www.jameco.com/Jameco/Products/ProdDS/207002.pdf

## Todo
- Wouldn't ideally have 0-1V across the thermistor constantly. Supply 3V3 on read only?
- Step down 5V Neopixel supply to 3V3
- Make it look better
- Make network requests while idling, not on recieving requests
