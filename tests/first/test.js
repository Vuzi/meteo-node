var demo = require('./build/Release/demo');

ret = demo();

if(ret == -1)
        console.log("Error with wiringPi");
else
        console.log("Everything works fine :)");

