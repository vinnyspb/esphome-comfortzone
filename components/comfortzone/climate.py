import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from . import (
    ComfortzoneComponent,
    comfortzone_ns,
)
from .const import (
    CONF_COMFORTZONE_ID,
    CONF_HEATPUMP_CLIMATE, CONF_WATER_HEATER_CLIMATE
)
from esphome.const import CONF_ID

CODEOWNERS = ["@vinnyspb"]
DEPENDENCIES = ["comfortzone"]

ComfortzoneHeatpumpClimate = comfortzone_ns.class_("ComfortzoneHeatpumpClimate", climate.Climate)
ComfortzoneWaterHeaterClimate = comfortzone_ns.class_("ComfortzoneWaterHeaterClimate", climate.Climate)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_COMFORTZONE_ID): cv.use_id(ComfortzoneComponent),
        cv.Optional(CONF_HEATPUMP_CLIMATE): climate.CLIMATE_SCHEMA.extend({
            cv.GenerateID(): cv.declare_id(ComfortzoneHeatpumpClimate),
        }),
        cv.Optional(CONF_WATER_HEATER_CLIMATE): climate.CLIMATE_SCHEMA.extend({
            cv.GenerateID(): cv.declare_id(ComfortzoneWaterHeaterClimate),
        }),
    }
)
CLIMATES_INIT = {
    CONF_HEATPUMP_CLIMATE: "set_heatpump_climate",
    CONF_WATER_HEATER_CLIMATE: "set_water_heater_climate",
}


async def to_code(config):
    hub = await cg.get_variable(config[CONF_COMFORTZONE_ID])

    for key, funcName in CLIMATES_INIT.items():
        if key in config:
            var_climate = cg.new_Pvariable(config[key][CONF_ID])
            await climate.register_climate(var_climate, config[key])
            cg.add(getattr(hub, funcName)(var_climate))
