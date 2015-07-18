var mongoose = require('mongoose');

mongoose.connect('mongodb://localhost/test');

exports.mongoose = mongoose;
var db = mongoose.connection;
db.on('error', console.error.bind(console, 'connection error:'));
