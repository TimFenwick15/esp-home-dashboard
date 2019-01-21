typedef enum {
  noWeather,
  sunny,
  cloudy,
  rainy,
  snowy,
  weatherMax
} weatherType;

String weatherText[weatherMax] = {
  String("No weather data"),
  String("Sunny"),
  String("Cloudy"),
  String("Rain"),
  String("Snow")
};