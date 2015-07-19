$(function () {
  $(document).ready(function () {
    var SensorsCharts = {};
    var numberTicks = 40;
    Highcharts.setOptions({
      global: {
        useUTC: false
      }
    });
    var socket = window.io(document.location.host);
    socket.connect();

    var $graphs = $('#graphs');

    function makeSensorType(sensor) {
      return sensor.type + '_' + sensor.sensor_type;
    }

    function makeChart(sensor, history) {
      var sensorType = makeSensorType(sensor);
      var graphId = 'graph-' + sensorType;
      var $graphContainer = $('<div class="graph" />');
      var $graph = $('<div id="' + graphId + '" style="width: 1000px; height: 400px"/>');
      $graphContainer.append($graph);
      $graphs.append($graphContainer);
      var data = history.filter(function (s) {
        return makeSensorType(s) === sensorType;
      }).map(function (s) {
        return {
          x: s.timestamp,
          y: s.value
        };
      }).sort(function (a, b) {
        if (a.x < b.x) { return -1; }
        if (a.x > b.x) { return 1; }
        return 0;
      });
      var res = (data.length > numberTicks) ? data.slice(data.length - numberTicks, numberTicks) : data;
      return $('#' + graphId).highcharts({
          chart: {
              type: 'spline',
              animation: Highcharts.svg, // don't animate in old IE
              marginRight: 10,
              events: {
                  load: function () {
                      var series = this.series[0];
                      socket.on(sensorType, function (data) {
                        console.log("Receive", sensorType, data);
                        series.addPoint([data.timestamp, data.value], true, series.length < numberTicks);
                      });
                  }
              }
          },
          title: {
              text: sensor.type + " via " + sensor.sensor_type
          },
          xAxis: {
              type: 'datetime',
              tickPixelInterval: 60
          },
          yAxis: {
              title: {
                  text: 'Value'
              },
              plotLines: [{
                  value: 0,
                  width: 1,
                  color: '#808080'
              }]
          },
          tooltip: {
              formatter: function () {
                  return '<b>' + this.series.name + '</b><br/>' +
                      Highcharts.dateFormat('%Y-%m-%d %H:%M:%S', this.x) + '<br/>' +
                      Highcharts.numberFormat(this.y, 2);
              }
          },
          legend: {
              enabled: false
          },
          exporting: {
              enabled: false
          },
          credits: {
              enabled: false
          },
          series: [{
              name: sensorType,
              data: res
          }]
      });
    }

    socket.on('historyLastHour', function (history) {
      console.log("historyLastHour", history);
      for (var i = history.length - 1; i >= 0; i--) {
        var sensor = history[i];
        if (!SensorsCharts[makeSensorType(sensor)]) {
          SensorsCharts[makeSensorType(sensor)] = makeChart(sensor, history);
        }
      }
      $('#graphs').show();
      $('.loader').hide();
    });
  });
});
