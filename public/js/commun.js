var highchartsColors = {
  Temperature: '#F44336',
  Light: '#FDD835',
  Pressure: '#546E7A',
  Humidity: '#1976D2'
};

var SensorsCharts = {};
var numberTicks = 60;

Highcharts.setOptions({
  global: {
    useUTC: false
  }
});

function makeSensorType(sensor) {
  return sensor.type + '_' + sensor.sensor_type;
}