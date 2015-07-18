var socket = window.io(document.location.host);

var temperaturesData = [
  { label: 'Temperature', values: [{time: new Date().getTime(), y: 0}] }
];

var areaChartInstance = $('#chart-temp').epoch({
    type: 'time.area',
    //pixelRatio: 1,
    data: temperaturesData,
    axes: ['top', 'right', 'bottom', 'left'],
    fps: 60
});

socket.connect();
socket.on('data', function (data) {
  console.log("on", data);
  if (data.data.type === 'Temperature') {
    console.log({
      time: data.data.time,
      y: data.data.value
    });
    temperaturesData[0].values.push({
      time: data.data.time,
      y: data.data.value
    });
    areaChartInstance.push([{
      time: data.data.time,
      y: data.data.value
    }]);
  }
});
