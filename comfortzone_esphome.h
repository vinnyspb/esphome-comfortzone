#include "esphome.h"
#include <cmath>
#include "comfortzone_heatpump.h"

namespace esphome::comfortzone {

static const char* TAG = "ComfortzoneComponent";

class ComfortzoneComponent : public Component, public UARTDevice {
 public:
  Sensor *flow_water_temperature = new Sensor();
  Sensor *current_compressor_input_power = new Sensor();
  unsigned long last_sent = 0;

  ComfortzoneComponent(UARTComponent *parent) : UARTDevice(parent) {}

  float get_setup_priority() const override { return esphome::setup_priority::DATA; }

  void setup() override {
    heatpump = new comfortzone_heatpump(this);
  }

  void loop() override {
    if(comfortzone_heatpump::PFT_NONE == heatpump->process()) {
      return;
    }

    unsigned long now = esp_timer_get_time() / 1000;
    if(now - last_sent >= 30000) {
      ESP_LOGD(TAG, "SENDING COMMAND");
      // ESP_LOGD(TAG, "COMMAND SENT: %d", heatpump->request_status06());
      last_sent = now;
    }

    if(heatpump->comfortzone_status.sensors_te1_flow_water_changed) {
      flow_water_temperature->publish_state((float)heatpump->comfortzone_status.sensors_te1_flow_water / 10.0);
      heatpump->comfortzone_status.sensors_te1_flow_water_changed = false;
    }

    if(heatpump->comfortzone_status.heatpump_current_compressor_input_power_changed) {
      current_compressor_input_power->publish_state(heatpump->comfortzone_status.heatpump_current_compressor_input_power);
      heatpump->comfortzone_status.heatpump_current_compressor_input_power_changed = false;
    }
  }

  void dump_config() override {
    ESP_LOGCONFIG(TAG, "Comfortzone initialized");
  }

private:
  comfortzone_heatpump* heatpump;
};

}
