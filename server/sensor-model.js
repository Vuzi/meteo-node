var mongoose = require('./database').mongoose;
var Schema = mongoose.Schema;
var Q = require('q');
var _ = require('lodash');
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

function makeType(sensor) {
  return sensor.type + '_' + sensor.sensor_type;
}

exports.findAllLastDay = function () {
  var deferred = Q.defer();
  SensorModel.find({
    query: {
      timestamp: {
        "$gte": new Date().getTime() - 3600000 * 24
      }
    }
  }).exec(function (err, result) {
    if (err) {
      deferred.reject(err);
    } else {
      var resultFiltered = result.reduce(function (acc, i) {
        var hour = new Date(i.timestamp).getHours();
        if (acc[makeType(i)]) {
          if(acc[makeType(i)][hour]) {
            acc[makeType(i)][hour].push(i);
          } else {
            acc[makeType(i)][hour] = [i];
          }
        } else {
          acc[makeType(i)] = {};
          acc[makeType(i)][hour] = [i];
        }
        return acc;
      }, {});

      var res = _.mapValues(resultFiltered, function (type) {
        return _.mapValues(type, function (sensorsByType) {
          var length = sensorsByType.length;
          var total = _.reduce(sensorsByType, function (acc, sensor) {
            acc.total += sensor.value;
            acc.value = acc.total / length;
            acc.type = sensor.type;
            acc.unit = sensor.unit;
            acc.unit_display = sensor.unit_display;
            acc.timestamp = sensor.timestamp;
            acc.sensor_type = sensor.sensor_type;
            acc.sensor_name = sensor.sensor_name;
            acc.date = sensor.date;
            return acc;
          }, { total: 0, value: 0} );
          return total;
        });
      });


      deferred.resolve(res);
    }
  });
  return deferred.promise;
};


