import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import (
    CONF_ID,
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
).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)