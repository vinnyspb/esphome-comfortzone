#pragma once

#include <cmath>
#include <vector>
#include <chrono>

#include "esphome/core/component.h"
#include "esphome/components/socket/socket.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/globals/globals_component.h"

#ifdef USE_API
#include "esphome/components/api/custom_api_device.h"
#endif

#ifdef USE_BINARY_SENSOR
#include "esphome/components/binary_sensor/binary_sensor.h"
#endif

#ifdef USE_CLIMATE
#include "esphome/components/climate/climate.h"
#endif

#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif

#ifdef USE_TEXT_SENSOR
#include "esphome/components/text_sensor/text_sensor.h"
#endif

class comfortzone_heatpump;

namespace esphome::comfortzone
{
#ifdef USE_CLIMATE
  class ComfortzoneHeatpumpClimate : public climate::Climate
  {
  public:
    void control(const climate::ClimateCall &call) override;
    climate::ClimateTraits traits() override;
  };

  class ComfortzoneWaterHeaterClimate : public climate::Climate
  {
  public:
    void control(const climate::ClimateCall &call) override;
    climate::ClimateTraits traits() override;
  };
#endif

  class ComfortzoneComponent :
#ifdef USE_API
      public api::CustomAPIDevice,
#endif
      public Component,
      public uart::UARTDevice
  {
  public:
    float get_setup_priority() const override;

    void set_te3_offset_global(globals::RestoringGlobalsComponent<float> *te3_offset);
    void set_re_de_pin(GPIOPin *re_de_pin);

#ifdef USE_SENSOR
    void set_fan_time_to_filter_change_sensor(sensor::Sensor *fan_time_to_filter_change);
    void set_te0_outdoor_temp_sensor(sensor::Sensor *sensors_te0_outdoor_temp);
    void set_te1_flow_water_sensor(sensor::Sensor *sensors_te1_flow_water);
    void set_te2_return_water_sensor(sensor::Sensor *sensors_te2_return_water);
    void set_te3_indoor_temp_sensor(sensor::Sensor *sensors_te3_indoor_temp);
    void set_te4_hot_gas_temp_sensor(sensor::Sensor *sensors_te4_hot_gas_temp);
    void set_te5_exchanger_out_sensor(sensor::Sensor *sensors_te5_exchanger_out);
    void set_te6_evaporator_in_sensor(sensor::Sensor *sensors_te6_evaporator_in);
    void set_te7_exhaust_air_sensor(sensor::Sensor *sensors_te7_exhaust_air);
    void set_te24_hot_water_temp_sensor(sensor::Sensor *sensors_te24_hot_water_temp);
    void set_current_compressor_frequency_sensor(sensor::Sensor *heatpump_current_compressor_frequency);
    void set_current_compressor_power_sensor(sensor::Sensor *heatpump_current_compressor_power);
    void set_current_add_power_sensor(sensor::Sensor *heatpump_current_add_power);
    void set_current_total_power_sensor(sensor::Sensor *heatpump_current_total_power);
    void set_current_compressor_input_power_sensor(sensor::Sensor *heatpump_current_compressor_input_power);
    void set_current_compressor_heating_input_power_sensor(sensor::Sensor *heatpump_current_compressor_heating_input_power);
    void set_current_compressor_water_input_power_sensor(sensor::Sensor *heatpump_current_compressor_water_input_power);
    void set_compressor_energy_sensor(sensor::Sensor *compressor_energy);
    void set_add_energy_sensor(sensor::Sensor *add_energy);
    void set_hot_water_energy_sensor(sensor::Sensor *hot_water_energy);
    void set_compressor_runtime_sensor(sensor::Sensor *compressor_runtime);
    void set_total_runtime_sensor(sensor::Sensor *total_runtime);
    void set_hot_water_setting_sensor(sensor::Sensor *hot_water_setting);
    void set_room_heating_setting_sensor(sensor::Sensor *room_heating_setting);
    void set_fan_speed_duty_sensor(sensor::Sensor *fan_speed_duty);
    void set_hot_water_calculated_setting_sensor(sensor::Sensor *hot_water_calculated_setting);
    void set_expansion_valve_calculated_setting_sensor(sensor::Sensor *expansion_valve_calculated_setting);
    void set_expansion_valve_temperature_difference_sensor(sensor::Sensor *expansion_valve_temperature_difference);
    void set_heating_cop_sensor(sensor::Sensor *heating_cop);
    void set_water_cop_sensor(sensor::Sensor *water_cop);
    void set_te3_indoor_temp_offset_sensor(sensor::Sensor *sensors_te3_indoor_temp_offset);
    void set_target_flow_water_temperature_sensor(sensor::Sensor *target_flow_water_temperature);
#endif

#ifdef USE_BINARY_SENSOR
    void set_filter_alarm_sensor(binary_sensor::BinarySensor *filter_alarm);
    void set_general_alarm_sensor(binary_sensor::BinarySensor *general_alarm);
    void set_hot_water_production_sensor(binary_sensor::BinarySensor *hot_water_production);
    void set_room_heating_in_progress_sensor(binary_sensor::BinarySensor *room_heating_in_progress);
    void set_additional_power_enabled_sensor(binary_sensor::BinarySensor *additional_power_enabled);
    void set_defrost_enabled_sensor(binary_sensor::BinarySensor *defrost_enabled);
    void set_extra_hot_water_setting_sensor(binary_sensor::BinarySensor *extra_hot_water_setting);
#endif

#ifdef USE_TEXT_SENSOR
    void set_compressor_activity_sensor(text_sensor::TextSensor *compressor_activity);
    void set_mode_sensor(text_sensor::TextSensor *mode);
    void set_fan_speed_sensor(text_sensor::TextSensor *fan_speed);
    void set_hot_water_priority_setting_sensor(text_sensor::TextSensor *hot_water_priority_setting);
#endif

#ifdef USE_CLIMATE
    void set_heatpump_climate(ComfortzoneHeatpumpClimate *heatpump_climate);
    void set_water_heater_climate(ComfortzoneWaterHeaterClimate *water_heater_climate);
#endif

    void dump_config() override;

#ifdef USE_API
    void debug_reroute(std::string ip, int port, int timeout);
    void set_sensor_offset(int sensor_num, float temp_offset); // sensor: [0:7], offset in °C (-10.0° -> 10.0°)
    void override_indoor_temperature(float temp);
    void enable_fireplace_mode();
    void disable_fireplace_mode();
#endif

    void setup() override;
    void loop() override;

  private:
    void disable_debugging();
    void forward_to_udp();
    void set_fireplace_mode(bool enable);

    globals::RestoringGlobalsComponent<float> *te3_offset_{nullptr};

    GPIOPin *re_de_pin_{nullptr};

#ifdef USE_SENSOR
    sensor::Sensor *fan_time_to_filter_change_{nullptr};   // days (proto: 1.60, 1.80)
    sensor::Sensor *sensors_te0_outdoor_temp_{nullptr};    // °C, * 10 (proto: 1.60, 1.80)
    sensor::Sensor *sensors_te1_flow_water_{nullptr};      // °C, * 10 (proto: 1.60, 1.80)
    sensor::Sensor *sensors_te2_return_water_{nullptr};    // °C, * 10 (proto: 1.60, 1.80)
    sensor::Sensor *sensors_te3_indoor_temp_{nullptr};     // °C, * 10 (proto: 1.60, 1.80)
    sensor::Sensor *sensors_te4_hot_gas_temp_{nullptr};    // °C, * 10 (proto: 1.60, 1.80)
    sensor::Sensor *sensors_te5_exchanger_out_{nullptr};   // °C, * 10 (proto: 1.60, 1.80)
    sensor::Sensor *sensors_te6_evaporator_in_{nullptr};   // °C, * 10 (proto: 1.60, 1.80)
    sensor::Sensor *sensors_te7_exhaust_air_{nullptr};     // °C, * 10 (proto: 1.60, 1.80)
    sensor::Sensor *sensors_te24_hot_water_temp_{nullptr}; // °C, * 10 (proto: 1.60, 1.80)

    sensor::Sensor *heatpump_current_compressor_frequency_{nullptr}; // Hz, * 10 (proto: 1.60, 1.80)

    sensor::Sensor *heatpump_current_compressor_power_{nullptr};               // W = power generated by compressor (proto: 1.60, 1.80)
    sensor::Sensor *heatpump_current_add_power_{nullptr};                      // W = additional power (resistor) (proto: 1.60, 1.80)
    sensor::Sensor *heatpump_current_total_power_{nullptr};                    // W = total power produced (proto: 1.60, 1.80)
    sensor::Sensor *heatpump_current_compressor_input_power_{nullptr};         // W = power sent to compressor (proto: 1.60, 1.80)
    sensor::Sensor *heatpump_current_compressor_heating_input_power_{nullptr}; // W = power sent to compressor (proto: 1.60, 1.80)
    sensor::Sensor *heatpump_current_compressor_water_input_power_{nullptr};   // W = power sent to compressor (proto: 1.60, 1.80)

    sensor::Sensor *compressor_energy_{nullptr}; // kWh, * 100 (proto: 1.60, 1.80)
    sensor::Sensor *add_energy_{nullptr};        // kWh, * 100 (proto: 1.60, 1.80)
    sensor::Sensor *hot_water_energy_{nullptr};  // kWh, * 100 (proto: 1.60, 1.80)

    sensor::Sensor *compressor_runtime_{nullptr}; // minutes (proto: 1.60, 1.80)
    sensor::Sensor *total_runtime_{nullptr};      // minutes (proto: 1.60, 1.80)

    sensor::Sensor *room_heating_setting_{nullptr}; // °C, * 10 (user selected) (proto: 1.60, 1.80)
    sensor::Sensor *hot_water_setting_{nullptr};    // °C, * 10 (user selected) (proto: 1.60, 1.80)

    // current heatpump calculated setting
    sensor::Sensor *fan_speed_duty_{nullptr};               // %, * 10 (proto: 1.60, 1.80)
    sensor::Sensor *hot_water_calculated_setting_{nullptr}; // °C, * 10 (heatpump selected). can be hot_water_setting (no extra hot water) or a different value (proto: 1.60, 1.80)

    sensor::Sensor *expansion_valve_calculated_setting_{nullptr};     // K, * 10 (proto: 1.60, 1.80)
    sensor::Sensor *expansion_valve_temperature_difference_{nullptr}; // K, * 10 (proto: 1.60, 1.80)

    sensor::Sensor *heating_cop_{nullptr};
    sensor::Sensor *water_cop_{nullptr};

    sensor::Sensor *sensors_te3_indoor_temp_offset_{nullptr};

    sensor::Sensor *target_flow_water_temperature_{nullptr};
#endif

#ifdef USE_BINARY_SENSOR
    binary_sensor::BinarySensor *filter_alarm_{nullptr}; // true = replace/clean filter alarm, false = filter ok (proto: 1.60, 1.80)
    binary_sensor::BinarySensor *general_alarm_{nullptr}; // true = any alarm except filter, false = no alarm (proto: 1.60, 1.80)

    binary_sensor::BinarySensor *hot_water_production_{nullptr};     // true = on, false = off
    binary_sensor::BinarySensor *room_heating_in_progress_{nullptr}; // true = on, false = off

    binary_sensor::BinarySensor *additional_power_enabled_{nullptr}; // true = resistor is enabled, false = resistor is disabled (proto: 1.60, 1.80)

    binary_sensor::BinarySensor *defrost_enabled_{nullptr};         // true = defrost in progress, false = no defrost in progress (proto: 1.60, 1.80)
    binary_sensor::BinarySensor *extra_hot_water_setting_{nullptr}; // true = on, false = off
#endif

#ifdef USE_TEXT_SENSOR
    text_sensor::TextSensor *compressor_activity_{nullptr}; // (proto: 1.60, 1.80)
    text_sensor::TextSensor *mode_{nullptr};                // (proto: 1.60, 1.80)

    // current user settings
    text_sensor::TextSensor *fan_speed_{nullptr}; // 1 = low, 2 = normal, 3 = fast

    text_sensor::TextSensor *hot_water_priority_setting_{nullptr}; // 1 = low, 2 = normal, 3 = fast
#endif

#ifdef USE_CLIMATE
    ComfortzoneHeatpumpClimate *heatpump_climate_{nullptr};
    ComfortzoneWaterHeaterClimate *water_heater_climate_{nullptr};
#endif

    // for debugging purposes
    uint8_t *grab_buffer_ = new uint8_t[256];
    uint16_t grab_frame_size_ = 0;
    std::chrono::system_clock::time_point debug_until_ = {};
    struct sockaddr_in src_addr_;
    struct sockaddr_in dest_addr_;
    std::unique_ptr<socket::Socket> sock_ = nullptr;

    comfortzone_heatpump *heatpump_;
    bool power_changed_ = false;

    std::chrono::system_clock::time_point last_indoor_temperature_override_ = {};
  };
}
