var express = require('express');
var app  = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var bodyParser = require('body-parser');
var MeteoNode = require('./plugin/build/Release/meteonetwork');
var SensorModel = require('./server/sensor-model');

app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

app.use(express.static(__dirname + '/public'));

app.get('/', function (req, res) {
  res.sendFile(__dirname + "/public/index.html");
});

var frequence = 6;

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
var lastClearTmp = new Date().getTime() - 3600000;
var averagesForStorage = {};

MeteoNode(function(sensor) {
  SensorModel.insertTmp(sensor).then(function () {
    console.log("Sensor stored tmp : ", makeSensorType(sensor), sensor.value, sensor.sensor_name);
  }).fail(function (err) {
    console.log("insertTmp err", err);
  });
  if (new Date().getTime() - lastClearTmp > 3600000) {
    SensorModel.clearTmp().then(function () {
      console.log("clearTmp");
      lastClearTmp = new Date().getTime();
    }).fail(function (err) {
      console.log("clearTmp err", err);
    });
  }
  sensorDataHndler.fire(sensor);
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
  var dataType = makeSensorType(data);
  if(averagesForStorage[dataType]) {
    averagesForStorage[dataType].value += data.value;
    averagesForStorage[dataType].count++;
  } else {
    averagesForStorage[dataType] = { value : data.value, count : 1 };
  }

  // Evry 10 minuts (10 * 10), Evry Hour (10 * 10 * 6)
  if(averagesForStorage[dataType].count >= (10 * 10)) {
    data.value = averagesForStorage[dataType].value / averagesForStorage[dataType].count;
    SensorModel.insert(data).then(function (sensor) {
      console.log("Sensor stored : ", makeSensorType(data), sensor.value, sensor.sensor_name, "handlers", sensorDataHndler.handlers.length);
    }).fail(function (err) {
      console.log(err);
    });
    averagesForStorage[dataType] = null;
  }
};

sensorDataHndler.subscribe(storeDataHandler);

function makeSensorType(sensor) {
  return sensor.type + '_' + sensor.sensor_type;
}

io.on('connection', function(socket) {
  
  socket.on('getHistoryLastHour', function () {
    SensorModel.findAllLastHour(5*60).then(function (sensors) {
      socket.emit('historyLastHour', sensors);
    }).fail(function (err) {
      console.log(err);
    });
  });

  socket.on('getHistoryLastDay', function (d) {
    var date = new Date(d);
    console.log('getHistoryLastDay', d, new Date(d));
    SensorModel.findDayData(date.getTime()).then(function (sensors) {
      socket.emit('historyLastDay', sensors);
    }).fail(function (err) {
      console.log(err);
    });
  });
  
  var handler = function (data) {
    console.log("Data sended : ", makeSensorType(data), data.value, data.sensor_name, "handlers", sensorDataHndler.handlers.length);
    socket.emit(makeSensorType(data), data);
  };

  sensorDataHndler.subscribe(handler);
  socket.on('disconnect', function() {
    console.log('disconnect', sensorDataHndler.handlers.length);
    sensorDataHndler.unsubscribe(handler);
  });
});

http.listen(80, function(){
  console.log('listening on ' + 80);
});
