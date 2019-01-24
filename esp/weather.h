typedef enum {
  noWeather,
  sunny,
  cloudy,
  rainy,
  snowy,
  weatherMax
} weatherType;

String weatherText[weatherMax] = {
  String("noweatherdata"),
  String("sunny"),
  String("cloudy"),
  String("rainy"),
  String("snowy")
};