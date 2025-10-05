#include "comfortzone_esphome.h"

#include <cmath>
#include <vector>
#include <chrono>
#include "comfortzone_heatpump.h"
#include "esphome_rs485.h"

#include "esphome/components/api/api_server.h"

namespace esphome::comfortzone
{
  static const char *TAG = "ComfortzoneComponent";

#ifdef USE_SENSOR
  static void on_temperature_sensor_update(int16_t value, void *context)
  {
    if (!context)
    {
      return;
    }
    auto sensor = (sensor::Sensor *)context;
    sensor->publish_state((float)value / 10.0);
  }

  static void on_frequency_sensor_update(int16_t value, void *context)
  {
    if (!context)
    {
      return;
    }
    auto sensor = (sensor::Sensor *)context;
    sensor->publish_state((float)value / 10.0);
  }

  static void on_power_sensor_update(int16_t value, void *context)
  {
    if (!context)
    {
      return;
    }
    auto sensor = (sensor::Sensor *)context;
    sensor->publish_state(value);
  }

  static void on_energy_sensor_update(uint32_t value, void *context)
  {
    if (!context)
    {
      return;
    }
    auto sensor = (sensor::Sensor *)context;
    sensor->publish_state((float)value / 100.0);
  }

  static void on_minutes_sensor_update(uint32_t value, void *context)
  {
    if (!context)
    {
      return;
    }
    auto sensor = (sensor::Sensor *)context;
    sensor->publish_state(value);
  }

  static void on_days_sensor_update(uint16_t value, void *context)
  {
    if (!context)
    {
      return;
    }
    auto sensor = (sensor::Sensor *)context;
    sensor->publish_state(value);
  }

  static void on_percentage_sensor_update(uint16_t value, void *context)
  {
    if (!context)
    {
      return;
    }
    auto sensor = (sensor::Sensor *)context;
    sensor->publish_state((float)value / 10.0);
  }
#endif

#ifdef USE_BINARY_SENSOR
  static void on_bool_sensor_update(bool value, void *context)
  {
    if (!context)
    {
      return;
    }
    auto sensor = (binary_sensor::BinarySensor *)context;
    sensor->publish_state(value);
  }
#endif

#ifdef USE_TEXT_SENSOR
  static void on_speed_sensor_update(byte value, void *context)
  {
    if (!context)
    {
      return;
    }
    auto sensor = (text_sensor::TextSensor *)context;
    if (value == 1)
    {
      sensor->publish_state("low");
    }
    else if (value == 2)
    {
      sensor->publish_state("normal");
    }
    else if (value == 3)
    {
      sensor->publish_state("fast");
    }
    else
    {
      sensor->publish_state("incorrect");
    }
  }

  static void on_mode_sensor_update(COMFORTZONE_MODE value, void *context)
  {
    if (!context)
    {
      return;
    }
    auto sensor = (text_sensor::TextSensor *)context;
    if (value == CZMD_IDLE)
    {
      sensor->publish_state("idle");
    }
    else if (value == CZMD_ROOM_HEATING)
    {
      sensor->publish_state("heating");
    }
    else if (value == CZMD_HOT_WATER)
    {
      sensor->publish_state("hot_water");
    }
#if HP_PROTOCOL == HP_PROTOCOL_1_8
    else if (value == CZMD_ROOM_HEATING_AND_HOT_WATER)
    {
      sensor->publish_state("heating_and_hot_water");
    }
#endif
    else
    {
      sensor->publish_state("incorrect");
    }
  }

  static void on_compressor_activity_sensor_update(COMFORTZONE_COMPRESSOR_ACTIVITY value, void *context)
  {
    if (!context)
    {
      return;
    }
    auto sensor = (text_sensor::TextSensor *)context;
    if (value == CZCMP_STOPPED)
    {
      sensor->publish_state("stopped");
    }
#if HP_PROTOCOL == HP_PROTOCOL_1_8
    else if (value == CZCMP_DEFROST)
    {
      sensor->publish_state("defrost");
    }
#endif
    else if (value == CZCMP_RUNNING)
    {
      sensor->publish_state("running");
    }
    else if (value == CZCMP_STOPPING)
    {
      sensor->publish_state("stopping");
    }
    else
    {
      sensor->publish_state("incorrect");
    }
  }
#endif

#ifdef USE_CLIMATE
  void ComfortzoneHeatpumpClimate::control(const climate::ClimateCall &call)
  {
    // if (call.get_mode().has_value())
    // {
    //   // User requested mode change
    //   ClimateMode mode = *call.get_mode();
    //   // Send mode to hardware
    //   // ...

    //   // Publish updated state
    //   this->mode = mode;
    //   this->publish_state();
    // }
    // if (call.get_target_temperature().has_value())
    // {
    //   // User requested target temperature change
    //   float temp = *call.get_target_temperature();
    //   // Send target temp to climate
    //   // ...
    // }
  }

  climate::ClimateTraits ComfortzoneHeatpumpClimate::traits()
  {
    // The capabilities of the climate device
    auto traits = climate::ClimateTraits();
    traits.set_supports_current_temperature(true);
    traits.set_supported_modes({climate::CLIMATE_MODE_HEAT, climate::CLIMATE_MODE_OFF});
    return traits;
  }

  void ComfortzoneWaterHeaterClimate::control(const climate::ClimateCall &call)
  {
    // if (call.get_mode().has_value())
    // {
    //   // User requested mode change
    //   ClimateMode mode = *call.get_mode();
    //   // Send mode to hardware
    //   // ...

    //   // Publish updated state
    //   this->mode = mode;
    //   this->publish_state();
    // }
    // if (call.get_target_temperature().has_value())
    // {
    //   // User requested target temperature change
    //   float temp = *call.get_target_temperature();
    //   // Send target temp to climate
    //   // ...
    // }
  }

  climate::ClimateTraits ComfortzoneWaterHeaterClimate::traits()
  {
    // The capabilities of the climate device
    auto traits = climate::ClimateTraits();
    traits.set_supports_current_temperature(true);
    traits.set_supported_modes({climate::CLIMATE_MODE_HEAT, climate::CLIMATE_MODE_OFF});
    return traits;
  }
#endif

  float ComfortzoneComponent::get_setup_priority() const { return esphome::setup_priority::DATA; }

  void ComfortzoneComponent::set_te3_offset_global(globals::RestoringGlobalsComponent<float> *te3_offset)
  {
    this->te3_offset_ = te3_offset;
  }

  void ComfortzoneComponent::set_re_de_pin(GPIOPin *re_de_pin) {
    this->re_de_pin_ = re_de_pin;

    // pull down the pin ASAP to avoid any communication with the heatpump
    this->re_de_pin_->digital_write(false);
  }

#ifdef USE_SENSOR
  void ComfortzoneComponent::set_fan_time_to_filter_change_sensor(sensor::Sensor *fan_time_to_filter_change) { this->fan_time_to_filter_change_ = fan_time_to_filter_change; }
  void ComfortzoneComponent::set_te0_outdoor_temp_sensor(sensor::Sensor *sensors_te0_outdoor_temp) { this->sensors_te0_outdoor_temp_ = sensors_te0_outdoor_temp; }
  void ComfortzoneComponent::set_te1_flow_water_sensor(sensor::Sensor *sensors_te1_flow_water) { this->sensors_te1_flow_water_ = sensors_te1_flow_water; }
  void ComfortzoneComponent::set_te2_return_water_sensor(sensor::Sensor *sensors_te2_return_water) { this->sensors_te2_return_water_ = sensors_te2_return_water; }
  void ComfortzoneComponent::set_te3_indoor_temp_sensor(sensor::Sensor *sensors_te3_indoor_temp) { this->sensors_te3_indoor_temp_ = sensors_te3_indoor_temp; }
  void ComfortzoneComponent::set_te4_hot_gas_temp_sensor(sensor::Sensor *sensors_te4_hot_gas_temp) { this->sensors_te4_hot_gas_temp_ = sensors_te4_hot_gas_temp; }
  void ComfortzoneComponent::set_te5_exchanger_out_sensor(sensor::Sensor *sensors_te5_exchanger_out) { this->sensors_te5_exchanger_out_ = sensors_te5_exchanger_out; }
  void ComfortzoneComponent::set_te6_evaporator_in_sensor(sensor::Sensor *sensors_te6_evaporator_in) { this->sensors_te6_evaporator_in_ = sensors_te6_evaporator_in; }
  void ComfortzoneComponent::set_te7_exhaust_air_sensor(sensor::Sensor *sensors_te7_exhaust_air) { this->sensors_te7_exhaust_air_ = sensors_te7_exhaust_air; }
  void ComfortzoneComponent::set_te24_hot_water_temp_sensor(sensor::Sensor *sensors_te24_hot_water_temp) { this->sensors_te24_hot_water_temp_ = sensors_te24_hot_water_temp; }
  void ComfortzoneComponent::set_current_compressor_frequency_sensor(sensor::Sensor *heatpump_current_compressor_frequency) { this->heatpump_current_compressor_frequency_ = heatpump_current_compressor_frequency; }
  void ComfortzoneComponent::set_current_compressor_power_sensor(sensor::Sensor *heatpump_current_compressor_power) { this->heatpump_current_compressor_power_ = heatpump_current_compressor_power; }
  void ComfortzoneComponent::set_current_add_power_sensor(sensor::Sensor *heatpump_current_add_power) { this->heatpump_current_add_power_ = heatpump_current_add_power; }
  void ComfortzoneComponent::set_current_total_power_sensor(sensor::Sensor *heatpump_current_total_power) { this->heatpump_current_total_power_ = heatpump_current_total_power; }
  void ComfortzoneComponent::set_current_compressor_input_power_sensor(sensor::Sensor *heatpump_current_compressor_input_power) { this->heatpump_current_compressor_input_power_ = heatpump_current_compressor_input_power; }
  void ComfortzoneComponent::set_current_compressor_heating_input_power_sensor(sensor::Sensor *heatpump_current_compressor_heating_input_power) { this->heatpump_current_compressor_heating_input_power_ = heatpump_current_compressor_heating_input_power; }
  void ComfortzoneComponent::set_current_compressor_water_input_power_sensor(sensor::Sensor *heatpump_current_compressor_water_input_power) { this->heatpump_current_compressor_water_input_power_ = heatpump_current_compressor_water_input_power; }
  void ComfortzoneComponent::set_compressor_energy_sensor(sensor::Sensor *compressor_energy) { this->compressor_energy_ = compressor_energy; }
  void ComfortzoneComponent::set_add_energy_sensor(sensor::Sensor *add_energy) { this->add_energy_ = add_energy; }
  void ComfortzoneComponent::set_hot_water_energy_sensor(sensor::Sensor *hot_water_energy) { this->hot_water_energy_ = hot_water_energy; }
  void ComfortzoneComponent::set_compressor_runtime_sensor(sensor::Sensor *compressor_runtime) { this->compressor_runtime_ = compressor_runtime; }
  void ComfortzoneComponent::set_total_runtime_sensor(sensor::Sensor *total_runtime) { this->total_runtime_ = total_runtime; }
  void ComfortzoneComponent::set_hot_water_setting_sensor(sensor::Sensor *hot_water_setting) { this->hot_water_setting_ = hot_water_setting; }
  void ComfortzoneComponent::set_room_heating_setting_sensor(sensor::Sensor *room_heating_setting) { this->room_heating_setting_ = room_heating_setting; }
  void ComfortzoneComponent::set_fan_speed_duty_sensor(sensor::Sensor *fan_speed_duty) { this->fan_speed_duty_ = fan_speed_duty; }
  void ComfortzoneComponent::set_hot_water_calculated_setting_sensor(sensor::Sensor *hot_water_calculated_setting) { this->hot_water_calculated_setting_ = hot_water_calculated_setting; }
  void ComfortzoneComponent::set_expansion_valve_calculated_setting_sensor(sensor::Sensor *expansion_valve_calculated_setting) { this->expansion_valve_calculated_setting_ = expansion_valve_calculated_setting; }
  void ComfortzoneComponent::set_expansion_valve_temperature_difference_sensor(sensor::Sensor *expansion_valve_temperature_difference) { this->expansion_valve_temperature_difference_ = expansion_valve_temperature_difference; }
  void ComfortzoneComponent::set_heating_cop_sensor(sensor::Sensor *heating_cop) { this->heating_cop_ = heating_cop; }
  void ComfortzoneComponent::set_water_cop_sensor(sensor::Sensor *water_cop) { this->water_cop_ = water_cop; }
  void ComfortzoneComponent::set_te3_indoor_temp_offset_sensor(sensor::Sensor *sensors_te3_indoor_temp_offset) { this->sensors_te3_indoor_temp_offset_ = sensors_te3_indoor_temp_offset; }
  void ComfortzoneComponent::set_target_flow_water_temperature_sensor(sensor::Sensor *target_flow_water_temperature) { this->target_flow_water_temperature_ = target_flow_water_temperature; }
#endif

#ifdef USE_BINARY_SENSOR
  void ComfortzoneComponent::set_filter_alarm_sensor(binary_sensor::BinarySensor *filter_alarm) { this->filter_alarm_ = filter_alarm; }
  void ComfortzoneComponent::set_general_alarm_sensor(binary_sensor::BinarySensor *general_alarm) { this->general_alarm_ = general_alarm; }
  void ComfortzoneComponent::set_hot_water_production_sensor(binary_sensor::BinarySensor *hot_water_production) { this->hot_water_production_ = hot_water_production; }
  void ComfortzoneComponent::set_room_heating_in_progress_sensor(binary_sensor::BinarySensor *room_heating_in_progress) { this->room_heating_in_progress_ = room_heating_in_progress; }
  void ComfortzoneComponent::set_additional_power_enabled_sensor(binary_sensor::BinarySensor *additional_power_enabled) { this->additional_power_enabled_ = additional_power_enabled; }
  void ComfortzoneComponent::set_defrost_enabled_sensor(binary_sensor::BinarySensor *defrost_enabled) { this->defrost_enabled_ = defrost_enabled; }
  void ComfortzoneComponent::set_extra_hot_water_setting_sensor(binary_sensor::BinarySensor *extra_hot_water_setting) { this->extra_hot_water_setting_ = extra_hot_water_setting; }
#endif

#ifdef USE_TEXT_SENSOR
  void ComfortzoneComponent::set_compressor_activity_sensor(text_sensor::TextSensor *compressor_activity) { this->compressor_activity_ = compressor_activity; }
  void ComfortzoneComponent::set_mode_sensor(text_sensor::TextSensor *mode) { this->mode_ = mode; }
  void ComfortzoneComponent::set_fan_speed_sensor(text_sensor::TextSensor *fan_speed) { this->fan_speed_ = fan_speed; }
  void ComfortzoneComponent::set_hot_water_priority_setting_sensor(text_sensor::TextSensor *hot_water_priority_setting) { this->hot_water_priority_setting_ = hot_water_priority_setting; }
#endif

#ifdef USE_CLIMATE
  void ComfortzoneComponent::set_heatpump_climate(ComfortzoneHeatpumpClimate *heatpump_climate) { this->heatpump_climate_ = heatpump_climate; }
  void ComfortzoneComponent::set_water_heater_climate(ComfortzoneWaterHeaterClimate *water_heater_climate) { this->water_heater_climate_ = water_heater_climate; }
#endif

  void ComfortzoneComponent::setup()
  {
    heatpump_ = new comfortzone_heatpump(new EspHomeRS485Interface(this, re_de_pin_));

    auto &status = heatpump_->comfortzone_status;

#ifdef USE_SENSOR
    status.sensors_te0_outdoor_temp.subscribe(on_temperature_sensor_update, sensors_te0_outdoor_temp_);
    status.sensors_te1_flow_water.subscribe(on_temperature_sensor_update, sensors_te1_flow_water_);
    status.sensors_te2_return_water.subscribe(on_temperature_sensor_update, sensors_te2_return_water_);
    status.sensors_te3_indoor_temp.subscribe(on_temperature_sensor_update, sensors_te3_indoor_temp_);
    status.sensors_te4_hot_gas_temp.subscribe(on_temperature_sensor_update, sensors_te4_hot_gas_temp_);
    status.sensors_te5_exchanger_out.subscribe(on_temperature_sensor_update, sensors_te5_exchanger_out_);
    status.sensors_te6_evaporator_in.subscribe(on_temperature_sensor_update, sensors_te6_evaporator_in_);
    status.sensors_te7_exhaust_air.subscribe(on_temperature_sensor_update, sensors_te7_exhaust_air_);
    status.sensors_te24_hot_water_temp.subscribe(on_temperature_sensor_update, sensors_te24_hot_water_temp_);

    status.expansion_valve_calculated_setting.subscribe(on_temperature_sensor_update, expansion_valve_calculated_setting_);
    status.expansion_valve_temperature_difference.subscribe(on_temperature_sensor_update, expansion_valve_temperature_difference_);

    status.heatpump_current_compressor_frequency.subscribe(on_frequency_sensor_update, heatpump_current_compressor_frequency_);
    status.heatpump_current_compressor_power.subscribe(on_power_sensor_update, heatpump_current_compressor_power_);
    status.heatpump_current_add_power.subscribe(on_power_sensor_update, heatpump_current_add_power_);
    status.heatpump_current_total_power.subscribe(on_power_sensor_update, heatpump_current_total_power_);
    status.heatpump_current_compressor_input_power.subscribe(on_power_sensor_update, heatpump_current_compressor_input_power_);

    status.compressor_energy.subscribe(on_energy_sensor_update, compressor_energy_);
    status.add_energy.subscribe(on_energy_sensor_update, add_energy_);
    status.hot_water_energy.subscribe(on_energy_sensor_update, hot_water_energy_);

    status.compressor_runtime.subscribe(on_minutes_sensor_update, compressor_runtime_);
    status.total_runtime.subscribe(on_minutes_sensor_update, total_runtime_);

    status.fan_speed_duty.subscribe(on_percentage_sensor_update, fan_speed_duty_);

    status.hot_water_calculated_setting.subscribe(on_temperature_sensor_update, hot_water_calculated_setting_);

    status.room_heating_setting.subscribe(on_temperature_sensor_update, room_heating_setting_);
    status.hot_water_setting.subscribe(on_temperature_sensor_update, hot_water_setting_);

    status.fan_time_to_filter_change.subscribe(on_days_sensor_update, fan_time_to_filter_change_);

    status.calculated_flow_set.subscribe(on_temperature_sensor_update, target_flow_water_temperature_);

    auto on_power_changed = [this](float)
    {
      power_changed_ = true;
    };
    heatpump_current_compressor_power_->add_on_state_callback(on_power_changed);
    heatpump_current_add_power_->add_on_state_callback(on_power_changed);
    heatpump_current_total_power_->add_on_state_callback(on_power_changed);
    heatpump_current_compressor_input_power_->add_on_state_callback(on_power_changed);

    if (te3_offset_ && sensors_te3_indoor_temp_offset_)
    {
      sensors_te3_indoor_temp_offset_->publish_state(id(te3_offset_));
    }
#endif

#ifdef USE_BINARY_SENSOR
    status.filter_alarm.subscribe(on_bool_sensor_update, filter_alarm_);
    status.general_alarm.subscribe(on_bool_sensor_update, general_alarm_);
    status.hot_water_production.subscribe(on_bool_sensor_update, hot_water_production_);
    status.room_heating_in_progress.subscribe(on_bool_sensor_update, room_heating_in_progress_);
    status.additional_power_enabled.subscribe(on_bool_sensor_update, additional_power_enabled_);

    status.extra_hot_water_setting.subscribe(on_bool_sensor_update, extra_hot_water_setting_);
    status.defrost_enabled.subscribe(on_bool_sensor_update, defrost_enabled_);
#endif

#ifdef USE_TEXT_SENSOR
    status.compressor_activity.subscribe(on_compressor_activity_sensor_update, compressor_activity_);
    status.mode.subscribe(on_mode_sensor_update, mode_);
    status.fan_speed.subscribe(on_speed_sensor_update, fan_speed_);
    status.hot_water_priority_setting.subscribe(on_speed_sensor_update, hot_water_priority_setting_);
#endif

#ifdef USE_CLIMATE
    if (sensors_te3_indoor_temp_)
    {
      sensors_te3_indoor_temp_->add_on_state_callback([this](float t)
                                                      {
        heatpump_climate_->current_temperature = t;
        heatpump_climate_->publish_state(); });
    }
    if (room_heating_setting_)
    {
      room_heating_setting_->add_on_state_callback([this](float t)
                                                   {
        heatpump_climate_->target_temperature = t;
        heatpump_climate_->publish_state(); });
    }
    if (room_heating_in_progress_)
    {
      room_heating_in_progress_->add_on_state_callback([this](bool in_progress)
                                                       {
        heatpump_climate_->mode = in_progress ? climate::CLIMATE_MODE_HEAT : climate::CLIMATE_MODE_OFF;
        heatpump_climate_->publish_state(); });
    }

    if (sensors_te24_hot_water_temp_)
    {
      sensors_te24_hot_water_temp_->add_on_state_callback([this](float t)
                                                          {
        water_heater_climate_->current_temperature = t;
        water_heater_climate_->publish_state(); });
    }
    if (hot_water_setting_)
    {
      hot_water_setting_->add_on_state_callback([this](float t)
                                                {
        water_heater_climate_->target_temperature = t;
        water_heater_climate_->publish_state(); });
    }
    if (hot_water_production_)
    {
      hot_water_production_->add_on_state_callback([this](bool in_progress)
                                                   {
        water_heater_climate_->mode = in_progress ? climate::CLIMATE_MODE_HEAT : climate::CLIMATE_MODE_OFF;
        water_heater_climate_->publish_state(); });
    }
#endif

    heatpump_->begin();
  }

  void ComfortzoneComponent::loop()
  {
    heatpump_->process();

#if defined(USE_BINARY_SENSOR) && defined(USE_TEXT_SENSOR) && defined(USE_SENSOR)
    if (power_changed_ && heatpump_current_compressor_input_power_ && heatpump_current_add_power_)
    {
      const float total_applied_power = heatpump_current_compressor_input_power_->get_state() + heatpump_current_add_power_->get_state();
      const float cop = heatpump_current_total_power_->get_state() / total_applied_power;

      if (additional_power_enabled_)
      {
        additional_power_enabled_->publish_state(heatpump_current_add_power_->get_state() > 0);
      }

      if (mode_ && heatpump_current_compressor_heating_input_power_ && heatpump_current_compressor_water_input_power_ &&
          heating_cop_ && water_cop_)
      {
        if (mode_->get_state() == "heating")
        {
          heatpump_current_compressor_heating_input_power_->publish_state(total_applied_power);
          heatpump_current_compressor_water_input_power_->publish_state(0);
          heating_cop_->publish_state(cop);
          water_cop_->publish_state(NAN);
        }
        else if (mode_->get_state() == "hot_water" || mode_->get_state() == "heating_and_hot_water")
        {
          heatpump_current_compressor_water_input_power_->publish_state(total_applied_power);
          heatpump_current_compressor_heating_input_power_->publish_state(0);
          water_cop_->publish_state(cop);
          heating_cop_->publish_state(NAN);
        }
        else if (mode_->get_state() == "idle")
        {
          heatpump_current_compressor_water_input_power_->publish_state(0);
          heatpump_current_compressor_heating_input_power_->publish_state(0);
          water_cop_->publish_state(NAN);
          heating_cop_->publish_state(NAN);
        }
      }
      power_changed_ = false;
    }
#endif

    if (debug_until_ > std::chrono::system_clock::time_point{})
    {
      if (std::chrono::system_clock::now() > debug_until_)
      {
        disable_debugging();
      }
      else
      {
        forward_to_udp();
      }
    }
  }

  void ComfortzoneComponent::disable_debugging()
  {
    debug_until_ = std::chrono::system_clock::time_point{};

    if (sock_)
    {
      ESP_LOGE(TAG, "Shutting down debug socket");
      sock_->shutdown(0);
      sock_->close();
      sock_ = nullptr;
      heatpump_->set_grab_buffer(nullptr, 0, nullptr);
    }
    ESP_LOGE(TAG, "Debugging disabled");
  }

  void ComfortzoneComponent::forward_to_udp()
  {
    int err = sock_->sendto(grab_buffer_, grab_frame_size_, 0, (struct sockaddr *)&dest_addr_, sizeof(dest_addr_));
    if (err < 0)
    {
      ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
      disable_debugging();
      return;
    }
  }

  void ComfortzoneComponent::dump_config()
  {
    ESP_LOGCONFIG(TAG, "Comfortzone initialized");
  }

#ifdef USE_API
  void ComfortzoneComponent::debug_reroute(std::string ip, int port, int timeout)
  {
    debug_until_ = std::chrono::system_clock::now() + std::chrono::seconds(timeout);
    heatpump_->set_grab_buffer(grab_buffer_, 256, &grab_frame_size_);

    memset(&src_addr_, 0, sizeof(src_addr_));
    src_addr_.sin_family = AF_INET;
    src_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
    src_addr_.sin_port = htons(9502);

    dest_addr_.sin_addr.s_addr = inet_addr(ip.c_str());
    dest_addr_.sin_family = AF_INET;
    dest_addr_.sin_port = htons(port);

    sock_ = socket::socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (!sock_)
    {
      ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
      disable_debugging();
      return;
    }

    if (sock_->bind((struct sockaddr *)&src_addr_, sizeof(src_addr_)) < 0)
    {
      ESP_LOGE(TAG, "Unable to bind socket: errno %d", errno);
      disable_debugging();
      return;
    }

    ESP_LOGI(TAG, "Debugging enabled, forwarding to %s:%d for %d seconds", ip.c_str(), port, timeout);
  }

  void ComfortzoneComponent::set_sensor_offset(int sensor_num, float temp_offset) // sensor: [0:7], offset in °C (-10.0° -> 10.0°)
  {
    if (sensor_num < 0 || sensor_num > 7)
    {
      ESP_LOGE(TAG, "Invalid sensor number %d", sensor_num);
      return;
    }

    if (temp_offset < -10.0 || temp_offset > 10.0)
    {
      ESP_LOGE(TAG, "Invalid temperature offset %f", temp_offset);
      return;
    }

    if (heatpump_->set_sensor_offset(sensor_num, temp_offset))
    {
      ESP_LOGI(TAG, "Set sensor %d offset to %f", sensor_num, temp_offset);
      return;
    }
    else
    {
      ESP_LOGE(TAG, "Failed to set sensor %d offset to %f", sensor_num, temp_offset);
      return;
    }
  }

  void ComfortzoneComponent::override_indoor_temperature(float temp)
  {
    if (!sensors_te3_indoor_temp_->has_state() || (std::chrono::system_clock::now() - last_indoor_temperature_override_) < std::chrono::minutes(5))
    {
      return;
    }

    const float offset = id(te3_offset_) + temp - sensors_te3_indoor_temp_->state;

    if (std::abs(offset - id(te3_offset_)) < 0.1)
    {
      return;
    }

    ESP_LOGE(TAG, "Calculated indoor temperature offset %.1f (heatpump reported %.1f, overriding with %.1f)", offset, sensors_te3_indoor_temp_->state, temp);

    if (heatpump_->set_sensor_offset(3, offset))
    {
      ESP_LOGE(TAG, "Set successful");
    }
    else
    {
      ESP_LOGE(TAG, "Failed");
    }

    last_indoor_temperature_override_ = std::chrono::system_clock::now();
    id(te3_offset_) = offset;
    sensors_te3_indoor_temp_offset_->publish_state(offset);
  }

  void ComfortzoneComponent::enable_fireplace_mode()
  {
    set_fireplace_mode(true);
  }

  void ComfortzoneComponent::disable_fireplace_mode()
  {
    set_fireplace_mode(false);
  }
#endif

  void ComfortzoneComponent::set_fireplace_mode(bool enable)
  {
    if (heatpump_->set_fireplace_mode(enable))
    {
      ESP_LOGI(TAG, "Fireplace mode: %d SUCCESS", static_cast<int>(enable));
    }
    else
    {
      ESP_LOGE(TAG, "Fireplace mode: %d FAILED", static_cast<int>(enable));
    }
  }
}
