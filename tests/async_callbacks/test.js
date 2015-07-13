/**
 * Vuzi
 * 
 * Bind a function to say 'hello world' every 3 seconds, and clear the callback after 10 seconds.
 * At the same time an incremented value i is printed, only to show that every call to the callback
 * is really asynchronous
 */

var delay = require('./build/Release/delay');
 
var i = 0;
 
setInterval(function() {
  console.log(i++);
},500);

setTimeout(function() {
  delay.cancel();
}, 10000);
 
// test the delay function
delay.delay(3,'hello world',function(a,b) {
  console.log('delay : ' + a + ',' + b);
});

