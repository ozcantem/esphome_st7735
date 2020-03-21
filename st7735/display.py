import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import display, spi
from esphome.const import CONF_DC_PIN, CONF_FULL_UPDATE_EVERY, \
    CONF_ID, CONF_LAMBDA, CONF_MODEL, CONF_PAGES, CONF_RESET_PIN, \
    CONF_ROTATION

DEPENDENCIES = ['spi']

CONF_LED_PIN = 'led_pin'

st7735_ns = cg.esphome_ns.namespace('st7735')
st7735 = st7735_ns.class_('ST7735', cg.PollingComponent, spi.SPIDevice,
                            display.DisplayBuffer)
ST7735_18_TFT = st7735_ns.class_('st7735_18_TFT', st7735)

ST7735Model = st7735_ns.enum('ST7735Model')

MODELS = {
    'TFT_1.8': ST7735Model.TFT_18,
}

ST7735_MODEL = cv.enum(MODELS, upper=True, space="_")

CONFIG_SCHEMA = cv.All(display.FULL_DISPLAY_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(st7735),
    cv.Required(CONF_MODEL): ST7735_MODEL,
    cv.Required(CONF_DC_PIN): pins.gpio_output_pin_schema,
    cv.Optional(CONF_RESET_PIN): pins.gpio_output_pin_schema,
    cv.Optional(CONF_LED_PIN): pins.gpio_output_pin_schema,
}).extend(cv.polling_component_schema('1s')).extend(spi.SPI_DEVICE_SCHEMA),
    cv.has_at_most_one_key(CONF_PAGES, CONF_LAMBDA))


def to_code(config):
    if config[CONF_MODEL] == 'TFT_1.8':
        lcd_type = ST7735_18_TFT
    rhs = lcd_type.new()
    var = cg.Pvariable(config[CONF_ID], rhs, type=lcd_type)

    yield cg.register_component(var, config)
    yield display.register_display(var, config)
    yield spi.register_spi_device(var, config)
    cg.add(var.set_model(config[CONF_MODEL]))

    dc = yield cg.gpio_pin_expression(config[CONF_DC_PIN])
    cg.add(var.set_dc_pin(dc))

    if CONF_LAMBDA in config:
        lambda_ = yield cg.process_lambda(config[CONF_LAMBDA], [(display.DisplayBufferRef, 'it')],
                                          return_type=cg.void)
        cg.add(var.set_writer(lambda_))
    if CONF_RESET_PIN in config:
        reset = yield cg.gpio_pin_expression(config[CONF_RESET_PIN])
        cg.add(var.set_reset_pin(reset))
    if CONF_LED_PIN in config:
        led_pin = yield cg.gpio_pin_expression(config[CONF_LED_PIN])
        cg.add(var.set_led_pin(led_pin))
