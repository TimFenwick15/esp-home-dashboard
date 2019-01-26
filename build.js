const minify = require('html-minifier').minify
const fs = require('fs')
const html = fs.readFileSync('index.html', 'utf8')
let result = minify(html, {
  removeComments: true,
  collapseWhitespace: true,
  minifyCSS: true
})

result = result.replace(/"/g, `'`)
result = `client.println(String("${result}`
result = result.replace(/TEMPLATE_TEMPERATURE/g, `") + data.temperature);client.println(String("`)
result = result.replace(/TEMPLATE_WEATHERTEMPERATURE/g, `") + data.weatherTemperature);client.println(String("`)
result = result.replace(/TEMPLATE_SHOWWEATHER/, `") + data.weather);client.println(String("`)
result = result + `"));`
result = `
#include <ESP8266WiFi.h>
typedef struct {
  int temperature;
  String weather;
  int weatherTemperature;
} sensorData;
void makeHTML(WiFiClient client, sensorData data) {
  ${result}
}
`
const output = `esp/html.h`
fs.writeFileSync(output, result)

console.log(`Successfully written to ${output}`)
/*console.log(`Verifying Arduino code...`)

const { exec } = require('child_process');
exec(`arduino_debug --verify esp/esp.ino`, (err, stdout, stderr) => {
  console.log(stdout)
  if (err) {
    console.log(stderr)
    console.log("The Arduino code did not compile successfully.")
  }
  else {
    console.log(`Arduino code compiled with no issues.`)
  }
})*/
