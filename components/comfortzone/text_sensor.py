import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import (
    ICON_HEATING_COIL, ICON_FAN, ICON_WATER
)
from . import (
    ComfortzoneComponent,
)
from .const import (
    CONF_COMFORTZONE_ID,
    CONF_COMPRESSOR_ACTIVITY,
    CONF_MODE,
    CONF_FAN_SPEED,
    CONF_HOT_WATER_PRIORITY_SETTING
)

CODEOWNERS = ["@vinnyspb"]
DEPENDENCIES = ["comfortzone"]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_COMFORTZONE_ID): cv.use_id(ComfortzoneComponent),
        cv.Optional(CONF_COMPRESSOR_ACTIVITY): text_sensor.text_sensor_schema(
            icon=ICON_HEATING_COIL,
        ),
        cv.Optional(CONF_MODE): text_sensor.text_sensor_schema(
            icon=ICON_HEATING_COIL,
        ),
        cv.Optional(CONF_FAN_SPEED): text_sensor.text_sensor_schema(
            icon=ICON_FAN,
        ),
        cv.Optional(CONF_HOT_WATER_PRIORITY_SETTING): text_sensor.text_sensor_schema(
            icon=ICON_WATER,
        ),
    }
)
TEXT_SENSORS_INIT = {
    CONF_COMPRESSOR_ACTIVITY: "set_compressor_activity_sensor",
    CONF_MODE: "set_mode_sensor",
    CONF_FAN_SPEED: "set_fan_speed_sensor",
    CONF_HOT_WATER_PRIORITY_SETTING: "set_hot_water_priority_setting_sensor",
}


async def to_code(config):
    hub = await cg.get_variable(config[CONF_COMFORTZONE_ID])

    for key, funcName in TEXT_SENSORS_INIT.items():
        if key in config:
            sens = await text_sensor.new_text_sensor(config[key])
            cg.add(getattr(hub, funcName)(sens))
