// test_new.js

/**
 * Load the MeteoNode module
 */
var MeteoNode = require('./build/Release/meteonode');

console.log('MeteoNode nodejs tests');

/**
 * Initialize MeteoNode module. Will return undefined
 * if everything is fine, and the error if anything goes
 * wrong.
 */
//var err = MeteoNode.init();
//if(err) {
//    console.log("Failed to init MeteoNode : ")
//    console.log(err);
//}

/**
 * Construct each objet for each sensors, any sensor
 * being defined by the passed informations. An
 * optionnal name can be provided (otherwise the
 * name will be the type of the element)
 */
var DHT22 = new MeteoNode.Sensor({
        type      : "DHT22",     // Type of sensor
        pin       : 0x7          // Pin for GPIO sensors
    }, "sensor_temp");

console.log(DHT22);

var TSL2561 = new MeteoNode.Sensor({
        type      : "TSL2561",   // Type of sensor
        address   : 0x39         // Address for i2c components
    }, "sensor_light");

console.log(TSL2561);

/**
 * Fetch data from the specified sensor, and call
 * the provided callback with it result values (each
 * time for each values, a sensor yieling one
 * or more values)
 */
DHT22.fetch(function(err, data) {
    if(err) {
        console.err("Error with DHT22 : ");
        console.err(err);
        return;
    }

    // Only log data for now
    console.log(data);
});

/**
 * Same as fetch, except that the callback
 * will be called every 3 seconds
 */
/*TSL2561.fetchInterval(function(err, data) {
    if(err) {
        console.err("Error with TSL2561 : ");
        console.err(err);
        return;
    }

    // Only log data for now
    console.log(data);
}, 3); // Call every 3 seconds
 */

// After 30s, stop the fetch interval.
/*setTimeout(function() {
    TSL2561.fetchClear();
}, 30000);*/

console.log('Control send back to the main thread !');
