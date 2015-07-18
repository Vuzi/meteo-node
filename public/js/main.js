var socket = window.io(document.location.host);
var TEMPERATURE = 'Temperature';
var PRESSURE = 'Pressure';
var HUMIDITY = 'Humidity';
var LIGHT = 'Light';

var temperaturesData = [
  { label: TEMPERATURE, values: [{time: new Date().getTime(), y: 0}] }
];

var pressureData = [
  { label: PRESSURE, values: [{time: new Date().getTime(), y: 0}] }
];

var humidityData = [
  { label: HUMIDITY, values: [{time: new Date().getTime(), y: 0}] }
];

var lightData = [
  { label: LIGHT, values: [{time: new Date().getTime(), y: 0}] }
];

var temperatureAreaChartInstance = $('#graph-temperature').epoch({
    type: 'time.line',
    //pixelRatio: 1,
    data: temperaturesData,
    bucketRange: [0, 100],
    axes: ['bottom', 'left'],
    fps: 60
});

var pressureAreaChartInstance = $('#graph-pressure').epoch({
    type: 'time.line',
    //pixelRatio: 1,
    data: temperaturesData,
    axes: ['bottom', 'left'],
    fps: 60
});

var humidityAreaChartInstance = $('#graph-humidity').epoch({
    type: 'time.line',
    //pixelRatio: 1,
    data: humidityData,
    axes: ['bottom', 'left'],
    fps: 60
});

var lightAreaChartInstance = $('#graph-light').epoch({
    type: 'time.line',
    //pixelRatio: 1,
    data: lightData,
    axes: ['bottom', 'left'],
    fps: 60
});

var SensorsDatas = {
  Temperature: {
    data: [{ label: TEMPERATURE, values: [{time: new Date().getTime(), y: 0}] }],
    graph: temperatureAreaChartInstance
  },
  Pressure: {
    data: [{ label: PRESSURE, values: [{time: new Date().getTime(), y: 0}] }],
    graph: pressureAreaChartInstance
  },
  Humidity:{
    data: [{ label: HUMIDITY, values: [{time: new Date().getTime(), y: 0}] }],
    graph: humidityAreaChartInstance
  },
  Light: {
    data: [{ label: LIGHT, values: [{time: new Date().getTime(), y: 0}] }],
    graph: lightAreaChartInstance
  }
};

socket.connect();

socket.on('history', function (history) {
  console.log("history", history);
  history.map(function (sensor) {
    if (SensorsDatas[sensor.type]) {
      SensorsDatas[sensor.type].data[0].values.push({
        time: sensor.time,
        y: sensor.value
      });
      SensorsDatas[sensor.type].graph.push([{
        time: sensor.time,
        y: sensor.value
      }]);
    } else {
      console.log("Error data unknown");
    }
  });
});

socket.on('data', function (data) {
  console.log("on data receive", data);
  if (SensorsDatas[data.data.type]) {
    SensorsDatas[data.data.type].data[0].values.push({
      time: data.data.time,
      y: data.data.value
    });
    SensorsDatas[data.data.type].graph.push([{
      time: data.data.time,
      y: data.data.value
    }]);
  } else {
    console.log("Error data unknown");
  }
});
