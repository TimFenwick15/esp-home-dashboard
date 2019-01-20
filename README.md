A webpage served by an Adafruit ESP8266 Feather Huzzah. Used to set colours on a Neopixel strip, and report temperature from a 503 thermistor.

index.html is minified and written to esp/html.h with "npm run build"

The esp.h webserver code is based on https://randomnerdtutorials.com/esp8266-web-server/ :heart:

Information on using thermistors was taken from https://arduino.stackexchange.com/questions/6875/how-to-use-503-thermistors-for-temperature :heart:

An example 503 thermistor data sheet can be found here: https://www.jameco.com/Jameco/Products/ProdDS/207002.pdf

Todo:
- Wouldn't ideally have 0-1V across the thermistor constantly. Supply 3V3 on read only?
- Step down 5V Neopixel supply to 3V3
