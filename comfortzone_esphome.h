#include "esphome.h"
#include <cmath>
#include <vector>
#include "comfortzone_heatpump.h"

namespace esphome::comfortzone {

static const char* TAG = "ComfortzoneComponent";

class ComfortzoneComponent : public Component, public UARTDevice {
 public:
  int de_pin;

  static ComfortzoneComponent* get_singleton(UARTComponent *parent) {
    if(singleton == nullptr) {
      singleton = new ComfortzoneComponent(parent, 37);
      App.register_component(singleton);
    }
    return singleton;
  }

  ComfortzoneComponent(UARTComponent *parent, int de_pin) : UARTDevice(parent), de_pin(de_pin) {
    heatpump = new comfortzone_heatpump(this, de_pin);
  }

  float get_setup_priority() const override { return esphome::setup_priority::DATA; }

  void setup() override {
    heatpump->setup();
  }

  void loop() override {
    if(comfortzone_heatpump::PFT_NONE == heatpump->process()) {
      return;
    }
  }

  std::vector<Sensor*> get_sensors() {
    auto& status = heatpump->comfortzone_status;
    return {
      status.fan_time_to_filter_change,
      status.sensors_te0_outdoor_temp,
      status.sensors_te1_flow_water,
      status.sensors_te2_return_water,
      status.sensors_te3_indoor_temp,
      status.sensors_te4_hot_gas_temp,
      status.sensors_te5_exchanger_out,
      status.sensors_te6_evaporator_in,
      status.sensors_te7_exhaust_air,
      status.sensors_te24_hot_water_temp,
      status.heatpump_current_compressor_frequency,
      status.heatpump_current_compressor_power,
      status.heatpump_current_add_power,
      status.heatpump_current_total_power,
      status.heatpump_current_compressor_input_power,
      status.compressor_energy,
      status.add_energy,
      status.hot_water_energy,
      status.compressor_runtime,
      status.total_runtime,
      status.room_heating_setting,
      status.hot_water_setting,
      status.led_luminosity_setting,
      status.fan_speed_duty,
      status.hot_water_calculated_setting
    };
  }

  std::vector<BinarySensor*> get_binary_sensors() {
    auto& status = heatpump->comfortzone_status;
    return {
      status.filter_alarm,
      status.hot_water_production,
      status.room_heating_in_progress,
      status.additional_power_enabled,
      status.defrost_enabled,
      status.extra_hot_water_setting
    };
  }

  std::vector<TextSensor*> get_text_sensors() {
    auto& status = heatpump->comfortzone_status;
    return {
      status.compressor_activity,
      status.mode,
      status.fan_speed,
      status.hot_water_priority_setting
    };
  }

  void dump_config() override {
    ESP_LOGCONFIG(TAG, "Comfortzone initialized");
  }

private:
  static ComfortzoneComponent* singleton;

  comfortzone_heatpump* heatpump;
};

ComfortzoneComponent* ComfortzoneComponent::singleton = nullptr;

}
