var mongoose = require('./database').mongoose;
var Schema = mongoose.Schema;
var Q = require('q');
var SensorSchema = new Schema({
    'type': String,
    'sensor_name': String,
    'sensor_type': String,
    'unit': String,
    'unit_display': String,
    'date': {
      'type': Date
    },
    'value': Number,
    'timestamp': Number
});
var SensorModel = mongoose.model('sensors', SensorSchema);

exports.insert = function (sensor) {
  var deferred = Q.defer();
  new SensorModel(sensor).save(function (err) {
    if (err) {
      deferred.reject(err);
    } else {
      deferred.resolve(sensor);
    }
  });
  return deferred.promise;
};
exports.findAllLastHour = function () {
  var deferred = Q.defer();
  SensorModel.find({
    timestamp: {
      "$gte": new Date().getTime() - 3600000
    }
  }).sort("-timestamp").limit(5*40+20).exec(function (err, sensors) {
    if (err) {
      deferred.reject(err);
    } else {
      deferred.resolve(sensors);
    }
  });
  return deferred.promise;
};
