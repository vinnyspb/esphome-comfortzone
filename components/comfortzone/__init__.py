import esphome.codegen as cg
import esphome.config_validation as cv
import esphome.pins as pins
from esphome.components import uart
from esphome.components.globals import GlobalsComponent
from esphome.const import (CONF_RESTORE_VALUE, CONF_INITIAL_VALUE)
from esphome.core import (CORE)
from .const import (
    CONF_COMFORTZONE_ID,
    CONF_RE_DE_PIN, CONF_TE3_OFFSET_GLOBAL_ID,
)

CODEOWNERS = ["@vinnyspb"]
DEPENDENCIES = ["uart"]

comfortzone_ns = cg.esphome_ns.namespace("comfortzone")
ComfortzoneComponent = comfortzone_ns.class_("ComfortzoneComponent", cg.Component, uart.UARTDevice)


def validate_global_float_with_restore_and_initial_value(value):
    defined_globals = CORE.raw_config.get('globals', {})
    global_var_conf = next((g for g in defined_globals if g.get('id') == value), None)
    if global_var_conf is None:
        raise cv.Invalid(f"Global ID '{value.id}' not found")

    if global_var_conf.get('type') != 'float':
        raise cv.Invalid(f"Global variable '{value.id}' must have type 'float'")

    if not global_var_conf.get(CONF_RESTORE_VALUE, False):
        raise cv.Invalid(f"Global variable '{value.id}' must have 'restore_value' set to true")

    if str(global_var_conf.get(CONF_INITIAL_VALUE, '')) != '0.0':
        raise cv.Invalid(f"Global variable '{value.id}' must have 'initial_value' set to '0.0'")

    return value


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_COMFORTZONE_ID): cv.declare_id(ComfortzoneComponent),
        cv.Required(CONF_RE_DE_PIN): pins.gpio_output_pin_schema,
        cv.Required(CONF_TE3_OFFSET_GLOBAL_ID): cv.All(
            # Ensure the ID refers to a global variable
            cv.use_id(GlobalsComponent),
            # Custom validator to enforce specific attributes about the global
            validate_global_float_with_restore_and_initial_value,
        ),
    }
).extend(uart.UART_DEVICE_SCHEMA)
FINAL_VALIDATE_SCHEMA = uart.final_validate_device_schema(
    "comfortzone", baud_rate=19200, require_tx=True, require_rx=True, data_bits=8, parity="NONE", stop_bits=1
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_COMFORTZONE_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_RE_DE_PIN in config and config[CONF_RE_DE_PIN]:
        re_de_pin = await cg.gpio_pin_expression(config[CONF_RE_DE_PIN])
        cg.add(var.set_re_de_pin(re_de_pin))

    if CONF_TE3_OFFSET_GLOBAL_ID in config:
        var_global_offset = await cg.get_variable(config[CONF_TE3_OFFSET_GLOBAL_ID])
        cg.add(var.set_te3_offset_global(var_global_offset))

    cg.add_library("FastCRC", version=None, repository="https://github.com/vinnyspb/FastCRC.git")
    cg.add_library("comfortzone_heatpump", version=None,
                   repository="https://github.com/qix67/comfortzone_heatpump.git")
