// hello.js
var addon = require('./build/Release/meteonetwork');

console.log('MeteoNetwork nodejs tests');
console.log('-----------------------------');
console.log('For now, this JS script will');
console.log('read on the pin 7 a DHT22 and');
console.log('call the provided callback');
console.log('with its results 5 times.');
console.log('-----------------------------');

addon(function(type, result) {
    console.log("Data received :");
    console.log(type + ' : ' + result.toFixed(2));
    console.log('-----------------------------');
}, {
    sensor_temp: {
        type: "DHT22",
        timeout: 10,
        pin: 7
    }/*,
    // TODO
    sensor_light: {
        type: "TSL2561",
        timeout: 10,
        pin: 7
    }*/
});

console.log('-----------------------------');

