
<p align="center">
  <img height="110" src="http://i.imgur.com/sdZ0aH5.png"/>
</p>

# MeteoNode
Nodejs plugin & server to read/display data from raspberryPi sensors
It uses raspi-sensor to easily fetch data from the raspberryPi sensors.

## Requirement
Of course, nodejs is required. Special requirements are also needed for the raspi-sensor module, which can be found here : https://github.com/Vuzi/raspi-sensors/blob/master/README.md

## Installation & configuration of the server
To install all the dependencies and configure the server, juste use 
````
npm install
````

You should now be able to run the server :
````
sudo node server.js
````
Note that, for now, you'll need to run node with root credentials because *raspi-sensor* needs it.

## Example
![](http://i.imgur.com/mFI8Agz.png)
