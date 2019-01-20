const minify = require('html-minifier').minify;
const fs = require('fs')
const html = fs.readFileSync('index.html', 'utf8')
let result = minify(html, {
  removeComments: true,
  collapseWhitespace: true,
  minifyCSS: true
});

result = result.replace(/"/g, `'`) 
result = result.split(`{{TEMPERATURE}}`)
result = `
typedef struct {
  int temperature;
} sensorData;
String makeHTML(sensorData data) {
  return String("${result[0]}") + data.temperature + String("${result[1]}");
}
`
const output = `esp/html.h`
fs.writeFileSync(output, result)
console.log(`Successfully written to ${output}`)
