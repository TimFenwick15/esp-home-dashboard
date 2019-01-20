A webpage served by an Adafruit ESP8266 Feather Huzzah. Used to set colours on a Neopixel strip, and report temperature from a 503 thermistor.

The esp.h webserver code is based on https://randomnerdtutorials.com/esp8266-web-server/ :heart:

Information on using thermistors was taken from https://arduino.stackexchange.com/questions/6875/how-to-use-503-thermistors-for-temperature :heart:

An example 503 thermistor data sheet can be found here: https://www.jameco.com/Jameco/Products/ProdDS/207002.pdf

Todo:
- Reading ADC in the same cycle as updating Neopixels lead to unexpected Neopixel colours. Temporary solution was to only read the ADC once. Make temperature update periodically?
- Wouldn't ideally have 0-1V across the thermistor constantly. Supply 3V3 on read only?

