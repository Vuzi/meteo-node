var MeteoNode = require('./build/Release/meteonode');

console.log('MeteoNode test v0.2');

var DHT22 = new MeteoNode.Sensor({
	type : "DHT22",
	pin  : 0X7
}); 

var TSL2561 = new MeteoNode.Sensor({
	type    : " TSL2561",
	address : 0X39
}, "light_sensor");

var BMP180 = new MeteoNode.Sensor({
	type    : "BMP180",
	address : 0x77
}, "temp_sensor");

DHT22.fetch(function(err, data) {
	console.log(err);
	console.log(data);
});

return;

TSL2561.fetchInterval(function(err, data) {
	console.log(data);
}, 4);

BMP180.fetchInterval(function(err, data) {
	console.log(data);
}, 5);

console.log('Control send back to the main thread');
