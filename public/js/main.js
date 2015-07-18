var socket = window.io(document.location.host);
var SensorsDatas = {};
socket.connect();

var $graphs = $('#graphs');

function buildChartInstance(sensorType, sensorsData) {
  var graphId = 'graph-' + sensorType;
  var $graphContainer = $('<div class="graph" />');
  var $title = $("<h2 />").append(sensorsData.type + " in " + sensorsData.unit + " (" + sensorsData.unit_display + ")" + " via " + sensorsData.sensor_type);
  var $graph = $('<div id="' + graphId + '" class="epoch" style="width: 500px; height: 200px"/>');
  $graphContainer.append($title);
  $graphContainer.append($graph);
  $graphs.append($graphContainer);
  return $('#' + graphId).epoch({
    type: 'time.line',
    windowSize: 60,
    queueSize: 120,
    historySize: 120,
    data: SensorsDatas[sensorType].data,
    bucketRange: [0, 100],
    axes: ['bottom', 'right'],
    fps: 60
  });
}

function makeSensorType(sensor) {
  return sensor.type + '_' + sensor.sensor_type;
}

socket.on('historyLastHour', function (history) {
  console.log("historyLastHour", history);
  for (var i = history.length - 1; i >= 0; i--) {
    var sensor = history[i];
    if (SensorsDatas[makeSensorType(sensor)]) {
      SensorsDatas[makeSensorType(sensor)].data[0].values.push({
        time: sensor.timestamp,
        y: sensor.value
      });
    } else {
      SensorsDatas[makeSensorType(sensor)] = {
        data: [{ label: sensor.type, values: [] }],
        type: sensor.type,
        unit: sensor.unit,
        unit_display: sensor.unit_display,
        sensor_type: sensor.sensor_type
      };
    }
  }
  console.log("SensorsDatas", SensorsDatas);
  for(var index in SensorsDatas) {
    var sensorsData = SensorsDatas[index];
    if (sensorsData.data[0].values.length === 0) {
      sensorsData.data[0].values.push({time: new Date().getTime(), y: 0});
    }
    sensorsData.graph = buildChartInstance(index, sensorsData);
  }
  console.log("SensorsDatas", SensorsDatas);
  $('#graphs').show();
  $('.loader').hide();
});

socket.on('data', function (data) {
  console.log("on data receive", data);
  if (SensorsDatas[makeSensorType(data)]) {
    SensorsDatas[makeSensorType(data)].data[0].values.push({
      time: data.timestamp,
      y: data.value
    });
    SensorsDatas[makeSensorType(data)].graph.push([{
      time: data.timestamp,
      y: data.value
    }]);
  } else {
    console.log("Error data unknown");
  }
});
