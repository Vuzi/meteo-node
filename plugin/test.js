// test.js
var addon = require('./build/Release/meteonode');

console.log('MeteoNode nodejs tests');
console.log('---------------------------------------');
console.log('For now, this JS script will read on : ');
console.log(' - DHT22 (pin 0x7 every 5s)');
console.log(' - TSL2561 (addr 0x39 every 2s');
console.log(' - BMP180 (addr 0x77 every 4s');
console.log(' - PIR (pin 0x11 every 1s');
console.log('And display the results.');
console.log('---------------------------------------');

addon(function(result) {
    console.log("Data received :");
    console.log(result);
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
    },
    sensor_pir : {
        type      : "PIR",
        frequence : 1,
        address   : 0x11
    }
});

console.log('Control send back to the main thread !');
