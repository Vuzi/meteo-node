# MeteoNode
Nodejs plugin & server to read/display data from raspberryPi sensors

## Plugin
The MeteoNode plugin can be used as a standalon in any other projet. It allows you to bind existing RaspberryPi sensors to any JavaScript code, using C++ to make low-level operations with sensors. To see how to build and use it, please refers to plugin/README.md .

## Requirement
Of course, nodejs is required. To see the requirement to build only the plugin, please refers to plugin/README.md .

## Installation & configuration of the server
To install all the dependencies and configure the server, juste use 
````
npm install
````

You should now be able to run the server :
````
sudo node server.js
````
Note that, for now, you'll need to run node with root credentials because *wiringPi* needs it in the plugin.
