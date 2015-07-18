var mongoose = require('./database').mongoose;
var Schema = mongoose.Schema;
var moment = require('moment');
var Q = require('q');
var SensorSchema = new Schema({
    'type': String,
    'name': String,
    'value': Number,
    'time': Number
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
    time: {
      "$gte": new Date().getTime() - 3600000
    }
  }).sort("-time").exec(function (err, sensors) {
    if (err) {
      deferred.reject(err);
    } else {
      deferred.resolve(sensors);
    }
  });
  return deferred.promise;
};
