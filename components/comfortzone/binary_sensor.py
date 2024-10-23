import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    ICON_FAN,
    DEVICE_CLASS_PROBLEM,
    ICON_WATER,
    ICON_HEATING_COIL,
    ICON_GRAIN,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_RUNNING
)
from . import (
    ComfortzoneComponent,
)
from .const import (
    CONF_COMFORTZONE_ID,
    CONF_FILTER_ALARM,
    CONF_GENERAL_ALARM,
    CONF_HOT_WATER_PRODUCTION,
    CONF_ROOM_HEATING_IN_PROGRESS,
    CONF_ADDITIONAL_POWER_ENABLED,
    CONF_DEFROST_ENABLED,
    CONF_EXTRA_HOT_WATER_SETTING
)

CODEOWNERS = ["@vinnyspb"]
DEPENDENCIES = ["comfortzone"]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_COMFORTZONE_ID): cv.use_id(ComfortzoneComponent),
        cv.Optional(CONF_FILTER_ALARM): binary_sensor.binary_sensor_schema(
            icon=ICON_FAN,
            device_class=DEVICE_CLASS_PROBLEM,
        ),
        cv.Optional(CONF_GENERAL_ALARM): binary_sensor.binary_sensor_schema(
            icon=ICON_HEATING_COIL,
            device_class=DEVICE_CLASS_PROBLEM,
        ),
        cv.Optional(CONF_HOT_WATER_PRODUCTION): binary_sensor.binary_sensor_schema(
            icon=ICON_WATER,
            device_class=DEVICE_CLASS_RUNNING,
        ),
        cv.Optional(CONF_ROOM_HEATING_IN_PROGRESS): binary_sensor.binary_sensor_schema(
            icon=ICON_HEATING_COIL,
            device_class=DEVICE_CLASS_RUNNING,
        ),
        cv.Optional(CONF_ADDITIONAL_POWER_ENABLED): binary_sensor.binary_sensor_schema(
            icon=ICON_HEATING_COIL,
            device_class=DEVICE_CLASS_POWER,
        ),
        cv.Optional(CONF_DEFROST_ENABLED): binary_sensor.binary_sensor_schema(
            icon=ICON_GRAIN,
            device_class=DEVICE_CLASS_RUNNING,
        ),
        cv.Optional(CONF_EXTRA_HOT_WATER_SETTING): binary_sensor.binary_sensor_schema(
            icon=ICON_HEATING_COIL,
            device_class=DEVICE_CLASS_POWER,
        ),
    }
)
BINARY_SENSORS_INIT = {
    CONF_FILTER_ALARM: "set_filter_alarm_sensor",
    CONF_GENERAL_ALARM: "set_general_alarm_sensor",
    CONF_HOT_WATER_PRODUCTION: "set_hot_water_production_sensor",
    CONF_ROOM_HEATING_IN_PROGRESS: "set_room_heating_in_progress_sensor",
    CONF_ADDITIONAL_POWER_ENABLED: "set_additional_power_enabled_sensor",
    CONF_DEFROST_ENABLED: "set_defrost_enabled_sensor",
    CONF_EXTRA_HOT_WATER_SETTING: "set_extra_hot_water_setting_sensor",
}


async def to_code(config):
    hub = await cg.get_variable(config[CONF_COMFORTZONE_ID])

    for key, funcName in BINARY_SENSORS_INIT.items():
        if key in config:
            sens = await binary_sensor.new_binary_sensor(config[key])
            cg.add(getattr(hub, funcName)(sens))
