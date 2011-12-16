var imagick = require('./imagick');
var fs = require('fs');

var i = new imagick.image();

//var blob = fs.readFileSync('./wizard.jpg');

i.readSync('./wizard.jpg')

i.rotate(90);
i.despeckle();
i.flip();
i.flop();

i.writeSync('./wizard.rotate.jpg');

var g = new imagick.geometry("50x50");
console.log(g.toString());
console.log('width', g.width);
console.log('height', g.height);
g.width = 75;
g.height = 100;
console.log(g.toString());


//fs.writeFileSync('./wizard.rotate.jpg', oblob);
