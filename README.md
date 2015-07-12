# MeteoNode
Nodejs plugin to read data from raspberryPi sensors

## Requirement
This plugin needs an existing installation of *wiringPi*. You should also generate a static library of *wiringPi* instead of the default shared one. To do so, juste run `make install-static` in the subdirectory 'wiringPi' at the root of *wiringPi*.

You'll also need *node-gyp*, g++/gcc and Make to generate the plugin.

## Building the plugin
Once everythin is installed, simply run :
````
node-gyp configure
node-gyp build
````

You should now be able to run the sort test 'test.js' :
````
sudo node test.js
````
Note that, for now, you'll need to run node with root credentials because *wiringPi* needs it.
