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
    type: 'time.area',
    //pixelRatio: 1,
    data: temperaturesData,
    bucketRange: [0, 100],
    axes: ['bottom', 'left'],
    fps: 60
});

var pressureAreaChartInstance = $('#graph-pressure').epoch({
    type: 'time.area',
    //pixelRatio: 1,
    data: temperaturesData,
    axes: ['bottom', 'left'],
    fps: 60
});

var humidityAreaChartInstance = $('#graph-humidity').epoch({
    type: 'time.area',
    //pixelRatio: 1,
    data: humidityData,
    axes: ['bottom', 'left'],
    fps: 60
});

var lightAreaChartInstance = $('#graph-light').epoch({
    type: 'time.area',
    //pixelRatio: 1,
    data: lightData,
    axes: ['bottom', 'left'],
    fps: 60
});

socket.connect();
socket.on('data', function (data) {
  console.log("on", data);
  switch(data.data.type) {
    case TEMPERATURE:
      temperaturesData[0].values.push({
        time: data.data.time,
        y: data.data.value
      });
      temperatureAreaChartInstance.push([{
        time: data.data.time,
        y: data.data.value
      }]);
      break;
    case PRESSURE:
      pressureData[0].values.push({
        time: data.data.time,
        y: data.data.value
      });
      pressureAreaChartInstance.push([{
        time: data.data.time,
        y: data.data.value
      }]);
      break;
    case HUMIDITY:
      humidityData[0].values.push({
        time: data.data.time,
        y: data.data.value
      });
      humidityAreaChartInstance.push([{
        time: data.data.time,
        y: data.data.value
      }]);
      break;
    case LIGHT:
      lightData[0].values.push({
        time: data.data.time,
        y: data.data.value
      });
      lightAreaChartInstance.push([{
        time: data.data.time,
        y: data.data.value
      }]);
      break;
  }
});
