var express = require('express');
var app  = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var bodyParser = require('body-parser');
var addon = require('./build/Release/meteonetwork');
var SensorModel = require('./server/sensor-model');

app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

app.use(express.static(__dirname + '/public'));

app.get('/', function (req, res) {
  res.sendFile(__dirname + "/public/index.html");
});

function SensorDataHndler() {
  this.handlers = []; // observers
}

SensorDataHndler.prototype = {
  subscribe: function(fn) {
    this.handlers.push(fn);
  },
  unsubscribe: function(fn) {
    this.handlers = this.handlers.filter(function(item) {
      if (item !== fn) { return item; }
    });
  },
  fire: function(o) {
    this.handlers.forEach(function(item) {
      item(o);
    });
  }
};

var sensorDataHndler = new SensorDataHndler();

var frequence = 60;

addon(function(data) {
  sensorDataHndler.fire(data);
}, {
    sensor_temp: {
        type      : "DHT22",
        frequence : frequence,
        pin       : 0x7
    },
    sensor_light: {
        type      : "TSL2561",
        frequence : frequence,
        address   : 0x39
    },
    sensor_press_temp : {
        type      : "BMP180",
        frequence : frequence,
        address   : 0x77
    }
});

var storeDataHandler = function (data) {
  SensorModel.insert(data).then(function (sensor) {
    console.log("Sensor stored : ", makeSensorType(data), sensor.value, sensor.sensor_name);
  }).fail(function (err) {
    console.log(err);
  });
};

sensorDataHndler.subscribe(storeDataHandler);

function makeSensorType(sensor) {
  return sensor.type + '_' + sensor.sensor_type;
}

io.on('connection', function(socket) {
  socket.on('getHistoryLastHour', function () {
    SensorModel.findAllLastHour().then(function (sensors) {
      socket.emit('historyLastHour', sensors);
    }).fail(function (err) {
      console.log(err);
    });
  });

  socket.on('getHistoryLastDay', function () {
    SensorModel.findAllLastDay().then(function (sensors) {
      socket.emit('historyLastDay', sensors);
    }).fail(function (err) {
      console.log(err);
    });
  });
  
  var handler = function (data) {
    console.log("Data sended : ", makeSensorType(data), data.value, data.sensor_name);
    socket.emit(makeSensorType(data), data);
  };
  sensorDataHndler.subscribe(handler);
  socket.on('disconnect', function() {
    sensorDataHndler.unsubscribe(handler);
  });
});

http.listen(80, function(){
  console.log('listening on ' + 80);
});
