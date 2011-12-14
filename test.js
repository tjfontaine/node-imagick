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


//fs.writeFileSync('./wizard.rotate.jpg', oblob);
