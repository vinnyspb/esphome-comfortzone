#include "comfortzone_esphome.h"

#include <cmath>
#include <vector>
#include <chrono>
#include "comfortzone_heatpump.h"
#include "esphome_rs485.h"

extern esphome::globals::RestoringGlobalsComponent<float> *te3_offset;

namespace esphome::comfortzone
{
  static const char *TAG = "ComfortzoneComponent";
  ComfortzoneComponent *ComfortzoneComponent::singleton = nullptr;

  static void on_temperature_sensor_update(int16_t value, void *context)
  {
    auto sensor = (Sensor *)context;
    sensor->publish_state((float)value / 10.0);
  }

  static void on_frequency_sensor_update(int16_t value, void *context)
  {
    auto sensor = (Sensor *)context;
    sensor->publish_state((float)value / 10.0);
  }

  static void on_power_sensor_update(int16_t value, void *context)
  {
    auto sensor = (Sensor *)context;
    sensor->publish_state(value);
  }

  static void on_bool_sensor_update(bool value, void *context)
  {
    auto sensor = (BinarySensor *)context;
    sensor->publish_state(value);
  }

  static void on_energy_sensor_update(uint32_t value, void *context)
  {
    auto sensor = (Sensor *)context;
    sensor->publish_state((float)value / 100.0);
  }

  static void on_minutes_sensor_update(uint32_t value, void *context)
  {
    auto sensor = (Sensor *)context;
    sensor->publish_state(value);
  }

  static void on_days_sensor_update(uint16_t value, void *context)
  {
    auto sensor = (Sensor *)context;
    sensor->publish_state(value);
  }

  static void on_percentage_sensor_update(uint16_t value, void *context)
  {
    auto sensor = (Sensor *)context;
    sensor->publish_state((float)value / 10.0);
  }

  static void on_speed_sensor_update(byte value, void *context)
  {
    auto sensor = (TextSensor *)context;
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
    auto sensor = (TextSensor *)context;
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
    else if (value == CZMD_ROOM_HEATING_AND_HOT_WATER)
    {
      sensor->publish_state("heating_and_hot_water");
    }
    else
    {
      sensor->publish_state("incorrect");
    }
  }

  static void on_compressor_activity_sensor_update(COMFORTZONE_COMPRESSOR_ACTIVITY value, void *context)
  {
    auto sensor = (TextSensor *)context;
    if (value == CZCMP_STOPPED)
    {
      sensor->publish_state("stopped");
    }
    else if (value == CZCMP_DEFROST)
    {
      sensor->publish_state("defrost");
    }
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

  void ComfortzoneHeatpumpClimate::control(const ClimateCall &call)
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

  ClimateTraits ComfortzoneHeatpumpClimate::traits()
  {
    // The capabilities of the climate device
    auto traits = climate::ClimateTraits();
    traits.set_supports_current_temperature(true);
    traits.set_supported_modes({climate::CLIMATE_MODE_HEAT, climate::CLIMATE_MODE_OFF});
    return traits;
  }

  void ComfortzoneWaterHeaterClimate::control(const ClimateCall &call)
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

  ClimateTraits ComfortzoneWaterHeaterClimate::traits()
  {
    // The capabilities of the climate device
    auto traits = climate::ClimateTraits();
    traits.set_supports_current_temperature(true);
    traits.set_supported_modes({climate::CLIMATE_MODE_HEAT, climate::CLIMATE_MODE_OFF});
    return traits;
  }

  ComfortzoneComponent *ComfortzoneComponent::get_singleton(UARTComponent *parent, int de_pin)
  {
    if (singleton == nullptr)
    {
      singleton = new ComfortzoneComponent(parent, de_pin);
      App.register_component(singleton);
    }
    return singleton;
  }

  ComfortzoneComponent::ComfortzoneComponent(UARTComponent *parent, int de_pin) : UARTDevice(parent)
  {
    heatpump = new comfortzone_heatpump(new EspHomeRS485Interface(this, de_pin));
  }

  float ComfortzoneComponent::get_setup_priority() const { return esphome::setup_priority::DATA; }

  void ComfortzoneComponent::setup()
  {
    auto &status = heatpump->comfortzone_status;

    status.sensors_te0_outdoor_temp.subscribe(on_temperature_sensor_update, sensors_te0_outdoor_temp);
    status.sensors_te1_flow_water.subscribe(on_temperature_sensor_update, sensors_te1_flow_water);
    status.sensors_te2_return_water.subscribe(on_temperature_sensor_update, sensors_te2_return_water);
    status.sensors_te3_indoor_temp.subscribe(on_temperature_sensor_update, sensors_te3_indoor_temp);
    status.sensors_te4_hot_gas_temp.subscribe(on_temperature_sensor_update, sensors_te4_hot_gas_temp);
    status.sensors_te5_exchanger_out.subscribe(on_temperature_sensor_update, sensors_te5_exchanger_out);
    status.sensors_te6_evaporator_in.subscribe(on_temperature_sensor_update, sensors_te6_evaporator_in);
    status.sensors_te7_exhaust_air.subscribe(on_temperature_sensor_update, sensors_te7_exhaust_air);
    status.sensors_te24_hot_water_temp.subscribe(on_temperature_sensor_update, sensors_te24_hot_water_temp);

    status.heatpump_current_compressor_frequency.subscribe(on_frequency_sensor_update, heatpump_current_compressor_frequency);
    status.heatpump_current_compressor_power.subscribe(on_power_sensor_update, heatpump_current_compressor_power);
    status.heatpump_current_add_power.subscribe(on_power_sensor_update, heatpump_current_add_power);
    status.heatpump_current_total_power.subscribe(on_power_sensor_update, heatpump_current_total_power);
    status.heatpump_current_compressor_input_power.subscribe(on_power_sensor_update, heatpump_current_compressor_input_power);

    status.compressor_energy.subscribe(on_energy_sensor_update, compressor_energy);
    status.add_energy.subscribe(on_energy_sensor_update, add_energy);
    status.hot_water_energy.subscribe(on_energy_sensor_update, hot_water_energy);

    status.compressor_runtime.subscribe(on_minutes_sensor_update, compressor_runtime);
    status.total_runtime.subscribe(on_minutes_sensor_update, total_runtime);

    status.mode.subscribe(on_mode_sensor_update, mode);
    status.compressor_activity.subscribe(on_compressor_activity_sensor_update, compressor_activity);
    status.filter_alarm.subscribe(on_bool_sensor_update, filter_alarm);
    status.hot_water_production.subscribe(on_bool_sensor_update, hot_water_production);
    status.room_heating_in_progress.subscribe(on_bool_sensor_update, room_heating_in_progress);
    status.additional_power_enabled.subscribe(on_bool_sensor_update, additional_power_enabled);

    status.fan_speed_duty.subscribe(on_percentage_sensor_update, fan_speed_duty);

    status.hot_water_calculated_setting.subscribe(on_temperature_sensor_update, hot_water_calculated_setting);

    status.fan_speed.subscribe(on_speed_sensor_update, fan_speed);
    status.room_heating_setting.subscribe(on_temperature_sensor_update, room_heating_setting);
    status.hot_water_setting.subscribe(on_temperature_sensor_update, hot_water_setting);
    status.extra_hot_water_setting.subscribe(on_bool_sensor_update, extra_hot_water_setting);
    status.defrost_enabled.subscribe(on_bool_sensor_update, defrost_enabled);
    status.hot_water_priority_setting.subscribe(on_speed_sensor_update, hot_water_priority_setting);

    status.fan_time_to_filter_change.subscribe(on_days_sensor_update, fan_time_to_filter_change);
    
    status.calculated_flow_set.subscribe(on_temperature_sensor_update, target_flow_water_temperature);

    auto on_power_changed = [this](float)
    {
      power_changed = true;
    };
    heatpump_current_compressor_power->add_on_state_callback(on_power_changed);
    heatpump_current_add_power->add_on_state_callback(on_power_changed);
    heatpump_current_total_power->add_on_state_callback(on_power_changed);
    heatpump_current_compressor_input_power->add_on_state_callback(on_power_changed);

    sensors_te3_indoor_temp->add_on_state_callback([this](float t)
                                                   {
        heatpump_climate->current_temperature = t;
        heatpump_climate->publish_state(); });
    room_heating_setting->add_on_state_callback([this](float t)
                                                {
        heatpump_climate->target_temperature = t;
        heatpump_climate->publish_state(); });
    room_heating_in_progress->add_on_state_callback([this](bool in_progress)
                                                    {
        heatpump_climate->mode = in_progress ? climate::CLIMATE_MODE_HEAT : climate::CLIMATE_MODE_OFF;
        heatpump_climate->publish_state(); });

    sensors_te24_hot_water_temp->add_on_state_callback([this](float t)
                                                       {
        water_heater_climate->current_temperature = t;
        water_heater_climate->publish_state(); });
    hot_water_setting->add_on_state_callback([this](float t)
                                             {
        water_heater_climate->target_temperature = t;
        water_heater_climate->publish_state(); });
    hot_water_production->add_on_state_callback([this](bool in_progress)
                                                {
        water_heater_climate->mode = in_progress ? climate::CLIMATE_MODE_HEAT : climate::CLIMATE_MODE_OFF;
        water_heater_climate->publish_state(); });

    sensors_te3_indoor_temp_offset->publish_state(id(te3_offset));

    heatpump->begin();
  }

  void ComfortzoneComponent::loop()
  {
    heatpump->process();

    if (power_changed)
    {
      const float total_applied_power = heatpump_current_compressor_input_power->get_state() + heatpump_current_add_power->get_state();
      const float cop = heatpump_current_total_power->get_state() / total_applied_power;

      additional_power_enabled->publish_state(heatpump_current_add_power->get_state() > 0);

      if (mode->get_state() == "heating")
      {
        heatpump_current_compressor_heating_input_power->publish_state(total_applied_power);
        heatpump_current_compressor_water_input_power->publish_state(0);
        heating_cop->publish_state(cop);
      }
      else if (mode->get_state() == "hot_water" || mode->get_state() == "heating_and_hot_water")
      {
        heatpump_current_compressor_water_input_power->publish_state(total_applied_power);
        heatpump_current_compressor_heating_input_power->publish_state(0);
        water_cop->publish_state(cop);
      }
      else if (mode->get_state() == "idle")
      {
        heatpump_current_compressor_water_input_power->publish_state(0);
        heatpump_current_compressor_heating_input_power->publish_state(0);
      }
      power_changed = false;
    }

    if (debug_until > std::chrono::system_clock::time_point{})
    {
      if (std::chrono::system_clock::now() > debug_until)
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
    debug_until = std::chrono::system_clock::time_point{};

    if (sock)
    {
      ESP_LOGE(TAG, "Shutting down debug socket");
      sock->shutdown(0);
      sock->close();
      sock = nullptr;
      heatpump->set_grab_buffer(nullptr, 0, nullptr);
    }
    ESP_LOGE(TAG, "Debugging disabled");
  }

  void ComfortzoneComponent::forward_to_udp()
  {
    int err = sock->sendto(grab_buffer, grab_frame_size, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err < 0)
    {
      ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
      disable_debugging();
      return;
    }
  }

  std::vector<Sensor *> ComfortzoneComponent::get_sensors()
  {
    return {
        fan_time_to_filter_change,
        sensors_te0_outdoor_temp,
        sensors_te1_flow_water,
        sensors_te2_return_water,
        sensors_te3_indoor_temp,
        sensors_te4_hot_gas_temp,
        sensors_te5_exchanger_out,
        sensors_te6_evaporator_in,
        sensors_te7_exhaust_air,
        sensors_te24_hot_water_temp,
        heatpump_current_compressor_frequency,
        heatpump_current_compressor_power,
        heatpump_current_add_power,
        heatpump_current_total_power,
        heatpump_current_compressor_input_power,
        heatpump_current_compressor_heating_input_power,
        heatpump_current_compressor_water_input_power,
        compressor_energy,
        add_energy,
        hot_water_energy,
        compressor_runtime,
        total_runtime,
        room_heating_setting,
        hot_water_setting,
        fan_speed_duty,
        hot_water_calculated_setting,
        heating_cop,
        water_cop,
        sensors_te3_indoor_temp_offset,
        target_flow_water_temperature};
  }

  std::vector<BinarySensor *> ComfortzoneComponent::get_binary_sensors()
  {
    return {
        filter_alarm,
        hot_water_production,
        room_heating_in_progress,
        additional_power_enabled,
        defrost_enabled,
        extra_hot_water_setting};
  }

  std::vector<TextSensor *> ComfortzoneComponent::get_text_sensors()
  {
    return {
        compressor_activity,
        mode,
        fan_speed,
        hot_water_priority_setting};
  }

  std::vector<Climate *> ComfortzoneComponent::get_climate_entities()
  {
    return {
        heatpump_climate,
        water_heater_climate};
  }

  void ComfortzoneComponent::dump_config()
  {
    ESP_LOGCONFIG(TAG, "Comfortzone initialized");
  }

  void ComfortzoneComponent::debug_reroute(const std::string &ip, int port, int timeout)
  {
    debug_until = std::chrono::system_clock::now() + std::chrono::seconds(timeout);
    heatpump->set_grab_buffer(grab_buffer, 256, &grab_frame_size);

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.sin_family = AF_INET;
    src_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    src_addr.sin_port = htons(9502);

    dest_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);

    sock = socket::socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (!sock)
    {
      ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
      disable_debugging();
      return;
    }

    if (sock->bind((struct sockaddr *)&src_addr, sizeof(src_addr)) < 0)
    {
      ESP_LOGE(TAG, "Unable to bind socket: errno %d", errno);
      disable_debugging();
      return;
    }

    ESP_LOGE(TAG, "Debugging enabled, forwarding to %s:%d for %d seconds", ip.c_str(), port, timeout);
  }

  bool ComfortzoneComponent::set_sensor_offset(int sensor_num, float temp_offset) // sensor: [0:7], offset in °C (-10.0° -> 10.0°)
  {
    if (sensor_num < 0 || sensor_num > 7)
    {
      ESP_LOGE(TAG, "Invalid sensor number %d", sensor_num);
      return false;
    }

    if (temp_offset < -10.0 || temp_offset > 10.0)
    {
      ESP_LOGE(TAG, "Invalid temperature offset %f", temp_offset);
      return false;
    }

    if (heatpump->set_sensor_offset(sensor_num, temp_offset))
    {
      ESP_LOGE(TAG, "Set sensor %d offset to %f", sensor_num, temp_offset);
      return true;
    }
    else
    {
      ESP_LOGE(TAG, "Failed to set sensor %d offset to %f", sensor_num, temp_offset);
      return false;
    }
  }

  void ComfortzoneComponent::override_indoor_temperature(float temp)
  {
    if (!sensors_te3_indoor_temp->has_state() || (std::chrono::system_clock::now() - last_indoor_temperature_override) < std::chrono::minutes(5))
    {
      return;
    }

    const float offset = id(te3_offset) + temp - sensors_te3_indoor_temp->state;

    if (std::abs(offset - id(te3_offset)) < 0.1)
    {
      return;
    }

    ESP_LOGE(TAG, "Calculated indoor temperature offset %.1f (heatpump reported %.1f, overriding with %.1f)", offset, sensors_te3_indoor_temp->state, temp);

    if (heatpump->set_sensor_offset(3, offset))
    {
      ESP_LOGE(TAG, "Set successful");
    }
    else
    {
      ESP_LOGE(TAG, "Failed");
    }

    last_indoor_temperature_override = std::chrono::system_clock::now();
    id(te3_offset) = offset;
    sensors_te3_indoor_temp_offset->publish_state(offset);
  }
}
