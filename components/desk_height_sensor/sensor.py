import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import (
    CONF_ID,
    CONF_UART_ID,
    UNIT_CENTIMETER,
    ICON_RULER,
    STATE_CLASS_MEASUREMENT,
)

DEPENDENCIES = ['uart']
AUTO_LOAD = ['sensor']

desk_height_ns = cg.esphome_ns.namespace('desk_height_sensor')
DeskHeightSensor = desk_height_ns.class_(
    'DeskHeightSensor', cg.Component, uart.UARTDevice, sensor.Sensor
)

CONFIG_SCHEMA = sensor.sensor_schema(
    DeskHeightSensor,
    unit_of_measurement=UNIT_CENTIMETER,
    icon=ICON_RULER,
    accuracy_decimals=1,
    state_class=STATE_CLASS_MEASUREMENT,
).extend({
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    uart_component = yield cg.get_variable(config[CONF_UART_ID])
    cg.add(var.set_uart_parent(uart_component))
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)
