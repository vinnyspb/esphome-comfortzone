import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    DEVICE_CLASS_DURATION,
    STATE_CLASS_TOTAL,
    ICON_FAN,
    UNIT_CELSIUS,
    ICON_THERMOMETER,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_HERTZ,
    ICON_POWER,
    DEVICE_CLASS_FREQUENCY,
    UNIT_WATT,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_POWER_FACTOR,
    UNIT_KILOWATT_HOURS,
    DEVICE_CLASS_ENERGY,
    UNIT_MINUTE,
    ICON_TIMER,
    UNIT_PERCENT,
    UNIT_KELVIN,
    ICON_HEATING_COIL
)
from . import (
    ComfortzoneComponent,
)
from .const import (
    CONF_COMFORTZONE_ID,
    CONF_FAN_TIME_TO_FILTER_CHANGE,
    CONF_TE0_OUTDOOR_TEMP,
    CONF_TE1_FLOW_WATER,
    CONF_TE2_RETURN_WATER,
    CONF_TE3_INDOOR_TEMP,
    CONF_TE4_HOT_GAS_TEMP,
    CONF_TE5_EXCHANGER_OUT,
    CONF_TE6_EVAPORATOR_IN,
    CONF_TE7_EXHAUST_AIR,
    CONF_TE24_HOT_WATER_TEMP,
    CONF_CURRENT_COMPRESSOR_FREQUENCY,
    CONF_CURRENT_COMPRESSOR_POWER,
    CONF_CURRENT_COMPRESSOR_ADDITIONAL_POWER,
    CONF_CURRENT_COMPRESSOR_TOTAL_POWER,
    CONF_CURRENT_COMPRESSOR_INPUT_POWER,
    CONF_CURRENT_COMPRESSOR_HEATING_INPUT_POWER,
    CONF_CURRENT_COMPRESSOR_WATER_INPUT_POWER,
    CONF_COMPRESSOR_ENERGY,
    CONF_ADD_ENERGY,
    CONF_HOT_WATER_ENERGY,
    CONF_COMPRESSOR_RUNTIME,
    CONF_TOTAL_RUNTIME,
    CONF_ROOM_HEATING_SETTING,
    CONF_HOT_WATER_SETTING,
    CONF_FAN_SPEED_DUTY,
    CONF_HOT_WATER_CALCULATED_SETTING,
    CONF_EXPANSION_VALVE_CALCULATED_SETTING,
    CONF_EXPANSION_VALVE_TEMPERATURE_DIFFERENCE,
    CONF_HEATING_COP,
    CONF_WATER_COP,
    CONF_TE3_INDOOR_TEMP_OFFSET,
    CONF_TARGET_FLOW_WATER_TEMPERATURE,
    UNIT_DAY
)

CODEOWNERS = ["@vinnyspb"]
DEPENDENCIES = ["comfortzone"]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_COMFORTZONE_ID): cv.use_id(ComfortzoneComponent),
        cv.Optional(CONF_FAN_TIME_TO_FILTER_CHANGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_DAY,
            icon=ICON_FAN,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_TOTAL,
        ),
        cv.Optional(CONF_TE0_OUTDOOR_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TE1_FLOW_WATER): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TE2_RETURN_WATER): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TE3_INDOOR_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TE4_HOT_GAS_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TE5_EXCHANGER_OUT): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TE6_EVAPORATOR_IN): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TE7_EXHAUST_AIR): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TE24_HOT_WATER_TEMP): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CURRENT_COMPRESSOR_FREQUENCY): sensor.sensor_schema(
            unit_of_measurement=UNIT_HERTZ,
            icon=ICON_POWER,
            device_class=DEVICE_CLASS_FREQUENCY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CURRENT_COMPRESSOR_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            icon=ICON_POWER,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CURRENT_COMPRESSOR_ADDITIONAL_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            icon=ICON_POWER,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CURRENT_COMPRESSOR_TOTAL_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            icon=ICON_POWER,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CURRENT_COMPRESSOR_INPUT_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            icon=ICON_POWER,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CURRENT_COMPRESSOR_HEATING_INPUT_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            icon=ICON_POWER,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CURRENT_COMPRESSOR_WATER_INPUT_POWER): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT,
            icon=ICON_POWER,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_COMPRESSOR_ENERGY): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOWATT_HOURS,
            icon=ICON_POWER,
            device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_TOTAL,
        ),
        cv.Optional(CONF_ADD_ENERGY): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOWATT_HOURS,
            icon=ICON_POWER,
            device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_TOTAL,
        ),
        cv.Optional(CONF_HOT_WATER_ENERGY): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOWATT_HOURS,
            icon=ICON_POWER,
            device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_TOTAL,
        ),
        cv.Optional(CONF_COMPRESSOR_RUNTIME): sensor.sensor_schema(
            unit_of_measurement=UNIT_MINUTE,
            icon=ICON_TIMER,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_TOTAL,
        ),
        cv.Optional(CONF_TOTAL_RUNTIME): sensor.sensor_schema(
            unit_of_measurement=UNIT_MINUTE,
            icon=ICON_TIMER,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_TOTAL,
        ),
        cv.Optional(CONF_ROOM_HEATING_SETTING): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_HOT_WATER_SETTING): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_FAN_SPEED_DUTY): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            icon=ICON_FAN,
            device_class=DEVICE_CLASS_POWER_FACTOR,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_HOT_WATER_CALCULATED_SETTING): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_EXPANSION_VALVE_CALCULATED_SETTING): sensor.sensor_schema(
            unit_of_measurement=UNIT_KELVIN,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_EXPANSION_VALVE_TEMPERATURE_DIFFERENCE): sensor.sensor_schema(
            unit_of_measurement=UNIT_KELVIN,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_HEATING_COP): sensor.sensor_schema(
            icon=ICON_HEATING_COIL,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_WATER_COP): sensor.sensor_schema(
            icon=ICON_HEATING_COIL,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TE3_INDOOR_TEMP_OFFSET): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TARGET_FLOW_WATER_TEMPERATURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_THERMOMETER,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
)
SENSORS_INIT = {
    CONF_FAN_TIME_TO_FILTER_CHANGE: "set_fan_time_to_filter_change_sensor",
    CONF_TE0_OUTDOOR_TEMP: "set_te0_outdoor_temp_sensor",
    CONF_TE1_FLOW_WATER: "set_te1_flow_water_sensor",
    CONF_TE2_RETURN_WATER: "set_te2_return_water_sensor",
    CONF_TE3_INDOOR_TEMP: "set_te3_indoor_temp_sensor",
    CONF_TE4_HOT_GAS_TEMP: "set_te4_hot_gas_temp_sensor",
    CONF_TE5_EXCHANGER_OUT: "set_te5_exchanger_out_sensor",
    CONF_TE6_EVAPORATOR_IN: "set_te6_evaporator_in_sensor",
    CONF_TE7_EXHAUST_AIR: "set_te7_exhaust_air_sensor",
    CONF_TE24_HOT_WATER_TEMP: "set_te24_hot_water_temp_sensor",
    CONF_CURRENT_COMPRESSOR_FREQUENCY: "set_current_compressor_frequency_sensor",
    CONF_CURRENT_COMPRESSOR_POWER: "set_current_compressor_power_sensor",
    CONF_CURRENT_COMPRESSOR_ADDITIONAL_POWER: "set_current_add_power_sensor",
    CONF_CURRENT_COMPRESSOR_TOTAL_POWER: "set_current_total_power_sensor",
    CONF_CURRENT_COMPRESSOR_INPUT_POWER: "set_current_compressor_input_power_sensor",
    CONF_CURRENT_COMPRESSOR_HEATING_INPUT_POWER: "set_current_compressor_heating_input_power_sensor",
    CONF_CURRENT_COMPRESSOR_WATER_INPUT_POWER: "set_current_compressor_water_input_power_sensor",
    CONF_COMPRESSOR_ENERGY: "set_compressor_energy_sensor",
    CONF_ADD_ENERGY: "set_add_energy_sensor",
    CONF_HOT_WATER_ENERGY: "set_hot_water_energy_sensor",
    CONF_COMPRESSOR_RUNTIME: "set_compressor_runtime_sensor",
    CONF_TOTAL_RUNTIME: "set_total_runtime_sensor",
    CONF_ROOM_HEATING_SETTING: "set_room_heating_setting_sensor",
    CONF_HOT_WATER_SETTING: "set_hot_water_setting_sensor",
    CONF_FAN_SPEED_DUTY: "set_fan_speed_duty_sensor",
    CONF_HOT_WATER_CALCULATED_SETTING: "set_hot_water_calculated_setting_sensor",
    CONF_EXPANSION_VALVE_CALCULATED_SETTING: "set_expansion_valve_calculated_setting_sensor",
    CONF_EXPANSION_VALVE_TEMPERATURE_DIFFERENCE: "set_expansion_valve_temperature_difference_sensor",
    CONF_HEATING_COP: "set_heating_cop_sensor",
    CONF_WATER_COP: "set_water_cop_sensor",
    CONF_TE3_INDOOR_TEMP_OFFSET: "set_te3_indoor_temp_offset_sensor",
    CONF_TARGET_FLOW_WATER_TEMPERATURE: "set_target_flow_water_temperature_sensor",
}


async def to_code(config):
    hub = await cg.get_variable(config[CONF_COMFORTZONE_ID])

    for key, funcName in SENSORS_INIT.items():
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(hub, funcName)(sens))
