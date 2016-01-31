# MeteoNode - Nodejs plugin
Nodejs C++ plugin, allowing to read data from raspberryPi's sensors

## Requirement
Nodejs is required. This actual version uses the *0.12.6* version of Nodejs, and may or may not be compatible with newest versions.

This plugin needs also an existing installation of *wiringPi*. The static library of *wiringPi* should be generated, instead of the default shared one. To do so, run `make install-static` in the subdirectory 'wiringPi' at the root of *wiringPi*.

Finally, *node-gyp*, *g++/gcc 4.8.2* and *Make* are also needed to compile and generate the plugin. You can install *node-gyp* with npm, and *g++/gcc* and *Make* with your favorite package manager.

You should also look at the tests files and modify the pin and addresses according to your own configuration, using *WiringPi* own [notation](http://wiringpi.com/pins/). If any sensor isn't connected, feel free to comment its initialization.

## Building the plugin
Once everything is installed, simply run :
````
node-gyp configure
node-gyp build
````
You should now be able to run the sensor test 'test.js' :
````
sudo node test.js
````
If your configuration is correct, you'll see some data from your sensors.

## Note
Note that, for now, root credentials are needed for any GPIO sensors because *wiringPi* needs it to access the GPIO bus.
