// hello.js
var addon = require('./build/Release/meteonetwork');

console.log('MeteoNetwork nodejs tests');
console.log('---------------------------------------');
console.log('For now, this JS script will read on : ');
console.log(' - DHT22 (pin 0x7 every 5s)');
console.log(' - TSL2561 (addr 0x39 every 2s');
console.log('And display the results.');
console.log('---------------------------------------');

addon(function(type, result) {
    console.log("Data received :");
    console.log(type + ' : ' + result.toFixed(2));
    console.log('---------------------------------------');
}, {
    sensor_temp: {
        type      : "DHT22",
        frequence : 5,
        pin       : 0x7
    },
    sensor_light: {
        type      : "TSL2561",
        frequence : 2,
        address   : 0x39
    },
    sensor_press_temp : {
        type      : "BMP180",
        frequence : 4,
        address   : 0x77
    }
});

console.log('Control send back to the main thread !');

