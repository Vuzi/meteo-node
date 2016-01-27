// test.js
var addon = require('./build/Release/meteonode');

console.log('MeteoNode nodejs tests');
console.log('---------------------------------------');
console.log('For now, this JS script will read on : ');
console.log(' - PIR (pin 0x0 every 1s');
console.log('And display the results.');
console.log('---------------------------------------');

addon(function(result) {
    if(result.value == 1)
        console.log("I can detect you!");
    else
        console.log("Nothing new captain!");
}, {
    sensor_pir : {
        type      : "PIR",
        frequence : 1,
        pin       : 0x0
    }
});

console.log('Control send back to the main thread !');
