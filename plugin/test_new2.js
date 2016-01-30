var MeteoNode = require('./build/Release/meteonode');

console.log("MeteoNode launched");

var PIR = new MeteoNode.Sensor({
	type : "DHT22",
	pin  : 0x7
}, "sensor_temp");

// Show every 5 second
//var pirInterval = PIR.fetchInterval(function(err, data) {
//	console.log(data);
//}, 5);

// Show every 10 second
//PIR.fetchInterval(function(err, data) {
//	console.log(data);
//}, 10);

// Show right now
//PIR.fetch(function(err, data) {
//	console.log(data);
//});

var t = setTimeout(function() {
	console.log("setTimeout");
	console.log(PIR);
//	PIR.fetchClear();
}, 5000);

console.log((process.memoryUsage().heapUsed / 1024 / 1024).toFixed(2), 'Mb');

setInterval(function() {
	console.log("en attente...");
	global.gc();
        console.log((process.memoryUsage().heapUsed / 1024 /1024).toFixed(2), 'Mb');
}, 2000);
