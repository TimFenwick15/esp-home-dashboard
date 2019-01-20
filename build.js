console.log('working')

// minify index.html
var minify = require('html-minifier').minify;
const fs = require('fs')
const html = fs.readFileSync('index.html', 'utf8')
var result = minify(html, {
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
fs.writeFileSync('esp/html.h', result)

"aaa".replace()

//console.log(result)

// split it on {{...}}

// #define first part

// #define second part

// This isn't extendable...

