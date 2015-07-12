{
  "targets": [
    {
      "target_name": "meteonetwork",
      "cflags": [ "-std=c++0x", "-Wall" ],
      "sources": [ "src/plugin.cc", "src/sensor.cc", "src/gpio_sensor.cc", "src/DHT22.cc", "src/sensor_result.cc" ],
      "include_dirs": [ "include" ],
      "libraries": [ "/usr/local/lib/libwiringPi.a" ]
    }
  ]
}

