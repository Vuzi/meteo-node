// hello.js
var addon = require('./build/Release/meteonetwork');

console.log('MeteoNetwork nodejs tests');
console.log('---------------------------------------');
console.log('For now, this JS script will read on : ');
console.log(' - DHT22 (pin 0x7)');
console.log(' - TSL2561 (addr 0x39');
console.log('10 reads at an interval of 5 seconds');
console.log('will be perfomed');
console.log('---------------------------------------');

addon(function(type, result) {
    console.log("Data received :");
    console.log(type + ' : ' + result.toFixed(2));
    console.log('---------------------------------------');
}, {
    sensor_temp: {
        type: "DHT22",
        timeout: 10,
        pin: 0x7
    },
    sensor_light: {
        type: "TSL2561",
        timeout: 10,
        address: 0x39
    }
});

console.log('---------------------------------------');

