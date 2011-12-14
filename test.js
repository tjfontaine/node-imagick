var imagick = require('./imagick');
var fs = require('fs');

var i = new imagick.image();

var blob = fs.readFileSync('./wizard.jpg');

i.readSync(blob).rotate(90)

var oblob = i.writeSync();

fs.writeFileSync('./wizard.rotate.jpg', oblob);
