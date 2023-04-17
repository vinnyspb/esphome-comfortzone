# Comfortzone EX50 decoder ported to ESPHome

This is a port of [comfortzone_heatpump](https://github.com/qix67/comfortzone_heatpump)
repository with the goal to run it on ESPHome / Home Assistant.
All the credits for the decoder part go to [@qix67](https://github.com/qix67/).

## Build and run

```
$ esphome compile comfortzone.yaml && esphome upload comfortzone.yaml && esphome logs comfortzone.yaml
```
