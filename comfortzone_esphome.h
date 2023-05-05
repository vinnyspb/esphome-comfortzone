#include "esphome.h"
#include <cmath>
#include <vector>
#include <chrono>
#include "comfortzone_heatpump.h"
#include "esphome_rs485.h"

namespace esphome::comfortzone
{
  static const char *TAG = "ComfortzoneComponent";

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

  class ComfortzoneHeatpumpClimate : public Climate
  {
  public:
    void control(const ClimateCall &call) override
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

    ClimateTraits traits() override
    {
      // The capabilities of the climate device
      auto traits = climate::ClimateTraits();
      traits.set_supports_current_temperature(true);
      traits.set_supported_modes({climate::CLIMATE_MODE_HEAT, climate::CLIMATE_MODE_OFF});
      return traits;
    }
  };

  class ComfortzoneWaterHeaterClimate : public Climate
  {
  public:
    void control(const ClimateCall &call) override
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

    ClimateTraits traits() override
    {
      // The capabilities of the climate device
      auto traits = climate::ClimateTraits();
      traits.set_supports_current_temperature(true);
      traits.set_supported_modes({climate::CLIMATE_MODE_HEAT, climate::CLIMATE_MODE_OFF});
      return traits;
    }
  };

  class ComfortzoneComponent : public Component, public UARTDevice
  {
  public:
    int de_pin;

    Sensor *fan_time_to_filter_change = new Sensor(); // days (proto: 1.60, 1.80)

    BinarySensor *filter_alarm = new BinarySensor(); // true = replace/clean filter alarm, false = filter ok (proto: 1.60, 1.80)

    BinarySensor *hot_water_production = new BinarySensor();     // true = on, false = off
    BinarySensor *room_heating_in_progress = new BinarySensor(); // true = on, false = off

    TextSensor *compressor_activity = new TextSensor();          // (proto: 1.60, 1.80)
    BinarySensor *additional_power_enabled = new BinarySensor(); // true = resistor is enabled, false = resistor is disabled (proto: 1.60, 1.80)

    TextSensor *mode = new TextSensor();                // (proto: 1.60, 1.80)
    BinarySensor *defrost_enabled = new BinarySensor(); // true = defrost in progress, false = no defrost in progress (proto: 1.60, 1.80)

    Sensor *sensors_te0_outdoor_temp = new Sensor();    // °C, * 10 (proto: 1.60, 1.80)
    Sensor *sensors_te1_flow_water = new Sensor();      // °C, * 10 (proto: 1.60, 1.80)
    Sensor *sensors_te2_return_water = new Sensor();    // °C, * 10 (proto: 1.60, 1.80)
    Sensor *sensors_te3_indoor_temp = new Sensor();     // °C, * 10 (proto: 1.60, 1.80)
    Sensor *sensors_te4_hot_gas_temp = new Sensor();    // °C, * 10 (proto: 1.60, 1.80)
    Sensor *sensors_te5_exchanger_out = new Sensor();   // °C, * 10 (proto: 1.60, 1.80)
    Sensor *sensors_te6_evaporator_in = new Sensor();   // °C, * 10 (proto: 1.60, 1.80)
    Sensor *sensors_te7_exhaust_air = new Sensor();     // °C, * 10 (proto: 1.60, 1.80)
    Sensor *sensors_te24_hot_water_temp = new Sensor(); // °C, * 10 (proto: 1.60, 1.80)

    Sensor *heatpump_current_compressor_frequency = new Sensor(); // Hz, * 10 (proto: 1.60, 1.80)

    Sensor *heatpump_current_compressor_power = new Sensor();               // W = power generated by compressor (proto: 1.60, 1.80)
    Sensor *heatpump_current_add_power = new Sensor();                      // W = additional power (resistor) (proto: 1.60, 1.80)
    Sensor *heatpump_current_total_power = new Sensor();                    // W = total power produced (proto: 1.60, 1.80)
    Sensor *heatpump_current_compressor_input_power = new Sensor();         // W = power sent to compressor (proto: 1.60, 1.80)
    Sensor *heatpump_current_compressor_heating_input_power = new Sensor(); // W = power sent to compressor (proto: 1.60, 1.80)
    Sensor *heatpump_current_compressor_water_input_power = new Sensor();   // W = power sent to compressor (proto: 1.60, 1.80)

    Sensor *compressor_energy = new Sensor(); // kWh, * 100 (proto: 1.60, 1.80)
    Sensor *add_energy = new Sensor();        // kWh, * 100 (proto: 1.60, 1.80)
    Sensor *hot_water_energy = new Sensor();  // kWh, * 100 (proto: 1.60, 1.80)

    Sensor *compressor_runtime = new Sensor(); // minutes (proto: 1.60, 1.80)
    Sensor *total_runtime = new Sensor();      // minutes (proto: 1.60, 1.80)

    // current user settings
    TextSensor *fan_speed = new TextSensor(); // 1 = low, 2 = normal, 3 = fast

    Sensor *room_heating_setting = new Sensor();                // °C, * 10 (user selected) (proto: 1.60, 1.80)
    Sensor *hot_water_setting = new Sensor();                   // °C, * 10 (user selected) (proto: 1.60, 1.80)
    BinarySensor *extra_hot_water_setting = new BinarySensor(); // true = on, false = off

    TextSensor *hot_water_priority_setting = new TextSensor(); // 1 = low, 2 = normal, 3 = fast

    // current heatpump calculated setting
    Sensor *fan_speed_duty = new Sensor();               // %, * 10 (proto: 1.60, 1.80)
    Sensor *hot_water_calculated_setting = new Sensor(); // °C, * 10 (heatpump selected). can be hot_water_setting (no extra hot water) or a different value (proto: 1.60, 1.80)

    Sensor *heating_cop = new Sensor();
    Sensor *water_cop = new Sensor();

    ComfortzoneHeatpumpClimate *heatpump_climate = new ComfortzoneHeatpumpClimate();
    ComfortzoneWaterHeaterClimate *water_heater_climate = new ComfortzoneWaterHeaterClimate();

    // for debugging purposes
    uint8_t *grab_buffer = new uint8_t[256];
    uint16_t grab_frame_size = 0;
    std::chrono::steady_clock::time_point debug_until = {};
    struct sockaddr_in src_addr;
    struct sockaddr_in dest_addr;
    std::unique_ptr<socket::Socket> sock = nullptr;

    static ComfortzoneComponent *get_singleton(UARTComponent *parent)
    {
      if (singleton == nullptr)
      {
        singleton = new ComfortzoneComponent(parent, 37);
        App.register_component(singleton);
      }
      return singleton;
    }

    ComfortzoneComponent(UARTComponent *parent, int de_pin) : UARTDevice(parent), de_pin(de_pin)
    {
      heatpump = new comfortzone_heatpump(new EspHomeRS485Interface(this, de_pin));
    }

    float get_setup_priority() const override { return esphome::setup_priority::DATA; }

    void setup() override
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

      heatpump->begin();
    }

    void loop() override
    {
      if (comfortzone_heatpump::PFT_NONE == heatpump->process())
      {
        return;
      }

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

      if (debug_until > std::chrono::steady_clock::time_point{})
      {
        if (std::chrono::steady_clock::now() > debug_until)
        {
          disable_debugging();
        }
        else
        {
          forward_to_udp();
        }
      }
    }

    void disable_debugging()
    {
      debug_until = std::chrono::steady_clock::time_point{};

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

    void forward_to_udp()
    {
      int err = sock->sendto(grab_buffer, grab_frame_size, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
      if (err < 0)
      {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
        disable_debugging();
        return;
      }
    }

    std::vector<Sensor *> get_sensors()
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
          water_cop};
    }

    std::vector<BinarySensor *> get_binary_sensors()
    {
      return {
          filter_alarm,
          hot_water_production,
          room_heating_in_progress,
          additional_power_enabled,
          defrost_enabled,
          extra_hot_water_setting};
    }

    std::vector<TextSensor *> get_text_sensors()
    {
      return {
          compressor_activity,
          mode,
          fan_speed,
          hot_water_priority_setting};
    }

    std::vector<Climate *> get_climate_entities()
    {
      return {
          heatpump_climate,
          water_heater_climate};
    }

    void dump_config() override
    {
      ESP_LOGCONFIG(TAG, "Comfortzone initialized");
    }

    void debug_reroute(const std::string &ip, int port, int timeout)
    {
      debug_until = std::chrono::steady_clock::now() + std::chrono::seconds(timeout);
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

      if(sock->bind((struct sockaddr *)&src_addr, sizeof(src_addr)) < 0) {
        ESP_LOGE(TAG, "Unable to bind socket: errno %d", errno);
        disable_debugging();
        return;
      }

      ESP_LOGE(TAG, "Debugging enabled, forwarding to %s:%d for %d seconds", ip.c_str(), port, timeout);
    }

  private:
    static ComfortzoneComponent *singleton;

    comfortzone_heatpump *heatpump;
    bool power_changed = false;
  };

  ComfortzoneComponent *ComfortzoneComponent::singleton = nullptr;
}
