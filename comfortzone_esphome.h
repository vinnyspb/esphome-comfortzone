#include "esphome.h"
#include <cmath>
#include "comfortzone_heatpump.h"

namespace esphome::comfortzone {

static const char* TAG = "ComfortzoneComponent";

class ComfortzoneComponent : public Component, public UARTDevice {
 public:
  Sensor *flow_water_temperature = new Sensor();
  Sensor *current_compressor_input_power = new Sensor();
  Sensor *current_compressor_power = new Sensor();
  Sensor *current_add_power = new Sensor();
  Sensor *current_total_power = new Sensor();
  Sensor *current_compressor_frequency = new Sensor();

  int de_pin;

  ComfortzoneComponent(UARTComponent *parent, int de_pin) : UARTDevice(parent), de_pin(de_pin) {}

  float get_setup_priority() const override { return esphome::setup_priority::DATA; }

  void setup() override {
    heatpump = new comfortzone_heatpump(this, de_pin);
  }

  void loop() override {
    if(comfortzone_heatpump::PFT_NONE == heatpump->process()) {
      return;
    }

    if(heatpump->comfortzone_status.sensors_te1_flow_water_changed) {
      flow_water_temperature->publish_state((float)heatpump->comfortzone_status.sensors_te1_flow_water / 10.0);
      heatpump->comfortzone_status.sensors_te1_flow_water_changed = false;
    }

    if(heatpump->comfortzone_status.heatpump_current_compressor_input_power_changed) {
      current_compressor_input_power->publish_state(heatpump->comfortzone_status.heatpump_current_compressor_input_power);
      heatpump->comfortzone_status.heatpump_current_compressor_input_power_changed = false;
    }

    if(heatpump->comfortzone_status.heatpump_current_compressor_frequency_changed) {
      current_compressor_frequency->publish_state((float)heatpump->comfortzone_status.heatpump_current_compressor_frequency / 10.0);
      heatpump->comfortzone_status.heatpump_current_compressor_frequency_changed = false;
    }

    if(heatpump->comfortzone_status.heatpump_current_compressor_power_changed) {
      current_compressor_power->publish_state(heatpump->comfortzone_status.heatpump_current_compressor_power);
      heatpump->comfortzone_status.heatpump_current_compressor_power_changed = false;
    }

    if(heatpump->comfortzone_status.heatpump_current_add_power_changed) {
      current_add_power->publish_state(heatpump->comfortzone_status.heatpump_current_add_power);
      heatpump->comfortzone_status.heatpump_current_add_power_changed = false;
    }

    if(heatpump->comfortzone_status.heatpump_current_total_power_changed) {
      current_total_power->publish_state(heatpump->comfortzone_status.heatpump_current_total_power);
      heatpump->comfortzone_status.heatpump_current_total_power_changed = false;
    }
  }

  void dump_config() override {
    ESP_LOGCONFIG(TAG, "Comfortzone initialized");
  }

private:
  comfortzone_heatpump* heatpump;
};

}
