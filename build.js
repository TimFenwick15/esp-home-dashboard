const minify = require('html-minifier').minify
const fs = require('fs')
const html = fs.readFileSync('index.html', 'utf8')
let result = minify(html, {
  removeComments: true,
  collapseWhitespace: true,
  minifyCSS: true
})

result = result.replace(/"/g, `'`)
result = result.replace(/TEMPLATE_TEMPERATURE/g, `") + data.temperature + String("`)
result = result.replace(/TEMPLATE_WEATHERTEMPERATURE/g, `") + data.weatherTemperature + String("`)
result = result.replace(/TEMPLATE_SHOWWEATHER/g, `") + data.weather + String("`)
result = `
typedef struct {
  int temperature;
  String weather;
  int weatherTemperature;
} sensorData;
String makeHTML(sensorData data) {
  return String("${result}");
}
`
const output = `esp/html.h`
fs.writeFileSync(output, result)

console.log(`Successfully written to ${output}`)
console.log(`Verifying Arduino code...`)

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
})
