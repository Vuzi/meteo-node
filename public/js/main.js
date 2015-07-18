var socket = window.io(document.location.host);
var TEMPERATURE = 'Temperature';
var PRESSURE = 'Pressure';
var HUMIDITY = 'Humidity';
var LIGHT = 'Light';

var SensorsDatas = {
  Temperature: {
    data: [{ label: TEMPERATURE, values: [] }]
  },
  Pressure: {
    data: [{ label: PRESSURE, values: [] }]
  },
  Humidity:{
    data: [{ label: HUMIDITY, values: [] }]
  },
  Light: {
    data: [{ label: LIGHT, values: [] }]
  }
};

socket.connect();

function builChartInstance(sensorType) {
  return $('#graph-' + sensorType).epoch({
    type: 'time.line',
    windowSize: 60,
    queueSize: 120,
    historySize: 120,
    data: SensorsDatas[sensorType].data,
    bucketRange: [0, 100],
    axes: ['bottom', 'left'],
    fps: 60
  });
}

socket.on('historyLastHour', function (history) {
  console.log("historyLastHour", history);
  for (var i = history.length - 1; i >= 0; i--) {
    var sensor = history[i];
    if (SensorsDatas[sensor.type]) {
      SensorsDatas[sensor.type].data[0].values.push({
        time: sensor.timestamp,
        y: sensor.value
      });
    } else {
      console.log("Error data unknown");
    }
  }
  if (SensorsDatas.Temperature.data[0].values.length === 0) {
    SensorsDatas.Temperature.data[0].values.push({time: new Date().getTime(), y: 0});
  }
  if (SensorsDatas.Pressure.data[0].values.length === 0) {
    SensorsDatas.Pressure.data[0].values.push({time: new Date().getTime(), y: 0});
  }
  if (SensorsDatas.Humidity.data[0].values.length === 0) {
    SensorsDatas.Humidity.data[0].values.push({time: new Date().getTime(), y: 0});
  }
  if (SensorsDatas.Light.data[0].values.length === 0) {
    SensorsDatas.Light.data[0].values.push({time: new Date().getTime(), y: 0});
  }
  SensorsDatas.Temperature.graph = builChartInstance(TEMPERATURE);
  SensorsDatas.Pressure.graph = builChartInstance(PRESSURE);
  SensorsDatas.Humidity.graph = builChartInstance(HUMIDITY);
  SensorsDatas.Light.graph = builChartInstance(LIGHT);
  $('#graphs').show();
  $('.loader').hide();
});

socket.on('data', function (data) {
  console.log("on data receive", data);
  if (SensorsDatas[data.type]) {
    SensorsDatas[data.type].data[0].values.push({
      time: data.timestamp,
      y: data.value
    });
    SensorsDatas[data.type].graph.push([{
      time: data.timestamp,
      y: data.value
    }]);
  } else {
    console.log("Error data unknown");
  }
});
