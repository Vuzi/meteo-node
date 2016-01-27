# MeteoNode - Nodejs plugin
Nodejs plugin to read data from raspberryPi sensors

## Requirement
Of course, nodejs is required.

This plugin needs also an existing installation of *wiringPi*. You should also generate a static library of *wiringPi* instead of the default shared one. To do so, juste run `make install-static` in the subdirectory 'wiringPi' at the root of *wiringPi*.

Finally, you'll also need *node-gyp*, *g++/gcc* and *Make* to generate the plugin. You can install *node-gyp* with npm.

You should also look at the tests files and modify the pin and addresses according to your own configuration, using *WiringPi* own [notation](http://wiringpi.com/pins/). If any sensor isn't connected, feel free to comment its initialization.

## Building the plugin
Once everything is installed, simply run :
````
node-gyp configure
node-gyp build
````
You should now be able to run the sensor test 'test.js' or the PIR sensor test 'test_pir.js' :
````
sudo node test.js
# or
sudo node test_pir.js
````
Note that, for now, you'll need to run node with root credentials because *wiringPi* needs it.
