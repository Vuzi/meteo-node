{
  "targets": [
    {
      "target_name": "meteonetwork",
      "cflags": [ "-std=c++0x", "-Wall" ],
      "sources": [ "plugin.cc", "sensor.cc", "gpio_sensor.cc", "DHT22.cc", "sensor_result.cc" ],
      "libraries": [ "/usr/local/lib/libwiringPi.a" ]
    }
  ]
}

