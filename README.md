# Comfortzone EX50 decoder ported to ESPHome

This is a port of [comfortzone_heatpump](https://github.com/qix67/comfortzone_heatpump)
repository with the goal to run it on ESPHome / Home Assistant.
All the credits for the decoder part go to [@qix67](https://github.com/qix67/).

## Hardware used

It can be run using almost any ESP32-compatible board with a RS485 module, but I used these:

- [Wemos S2 mini](https://www.wemos.cc/en/latest/s2/s2_mini.html)
- [RS485 module](https://www.aliexpress.com/item/32846149743.html)

## Build and run

```
$ esphome compile comfortzone.yaml && esphome upload comfortzone.yaml && esphome logs comfortzone.yaml
```
