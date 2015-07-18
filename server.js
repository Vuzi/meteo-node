var express = require('express');
var moment = require('moment');
var app  = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var bodyParser = require('body-parser');
//var addon = require('./build/Release/meteonetwork');

app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

app.use(express.static(__dirname + '/public'));

app.get('/', function (req, res) {
  res.sendFile(__dirname + "/public/index.html");
});

// addon(function(type, result) {
//     console.log("Data received :");
//     console.log(type + ' : ' + result.toFixed(2));
//     io.broadcast.emit(data, {
//         type: type,
//         value: result.toFixed(2)
//     });
//     console.log('---------------------------------------');
// }, {
//     sensor_temp: {
//         type      : "DHT22",
//         frequence : 5,
//         pin       : 0x7
//     },
//     sensor_light: {
//         type      : "TSL2561",
//         frequence : 2,
//         address   : 0x39
//     },
//     sensor_press_temp : {
//         type      : "BMP180",
//         frequence : 4,
//         address   : 0x77
//     }
// });

io.on('connection', function(socket) {
  setInterval(function () {
    socket.emit("data", {
      data: {
        type: "temperature",
        value: Math.random() * 50,
        time: new Date().getTime()
      }
    });
}, 2000);
});

http.listen(8011, function(){
  console.log('listening on ' + 8011);
});
