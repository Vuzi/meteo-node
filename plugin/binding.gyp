{
  "targets": [
    {
      "target_name": "meteonode",
      "cflags": [ "-std=c++0x", "-Wall" ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [
	      	"src/plugin.cc",
	      	"src/sensor.cc",
	      	"src/gpio_sensor.cc",
	      	"src/DHT22.cc",
                "src/PIR.cc",
	      	"src/i2c_sensor.cc",
	      	"src/TSL2561.cc",
	      	"src/BMP180.cc",
	      	"src/sensor_result.cc",
	      	"src/scheduler.cc"
      	],
      "include_dirs": [ "include" ],
      "libraries": [ "/usr/local/lib/libwiringPi.a" ]
    }
  ]
}

