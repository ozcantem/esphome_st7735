# esphome components :tada:
## st7735

This component allows you to use ST7735 display with ESP32. Thanks to [mvturnho](https://github.com/mvturnho/esphome/tree/ili9341) and [musk95](https://github.com/musk95/esphome), the codes has been modified in accordance with st7735. 

## To use this component
- Copy display, image and st7735 folders to the component folder of esphome.

![image](https://user-images.githubusercontent.com/4086591/77848839-43a14480-71d0-11ea-96c7-28a54f5b6681.png)

## sample configuration
```yaml
image:
  - file: "images/weather.png"
    id: weather
    resize: 64x64
    type: "RGB565"
  - file: "images/signal.png"
    id: signal
    resize: 64x64
    type: "RGB565"

sensor:
  - platform: wifi_signal
    name: "WiFi Signal Sensor"
    update_interval: 60s
    id: wifisignal

# Example configuration entry
text_sensor:
  - platform: homeassistant
    id: weather_home
    name: "Weather" 
    entity_id: weather.home   
  - platform: homeassistant
    id: home_temperature
    name: "Home Temperature"
    entity_id: sensor.temperature
  - platform: wifi_info
    ip_address:
      name: ESP IP Address
      id: espip
    ssid:
      name: ESP Connected SSID
      id: espssid

font:
  - file: "fonts/conthrax-sb.ttf"
    id: my_font
    size: 12

spi:
  clk_pin: GPIO18
  mosi_pin: GPIO23

display:
  - platform: st7735
    model: TFT_1.8
    cs_pin: GPIO22
    dc_pin: GPIO21
    reset_pin: GPIO19
    led_pin: GPIO5
    rotation: 90
    id: ha_display
    pages:
      - id: pageSignal
        lambda: |-
          it.filled_rectangle(0,0,160,128,st7735::ST7735_BLACK);
          it.image(5, 5, id(signal));
          if (id(wifisignal).has_state()) {           
            it.printf(15, 75,id(my_font), st7735::ST7735_BLUE, "%.0f dB",id(wifisignal).state);
            it.printf(15, 95,id(my_font), st7735::ST7735_BLUE,"IP: %s",id(espip).state.c_str());
            it.printf(15, 115,id(my_font), st7735::ST7735_BLUE,"SSID: %s",id(espssid).state.c_str());
          }
      - id: pageWeather
        lambda: |-
          it.filled_rectangle(0,0,160,128,st7735::ST7735_BLACK);
          it.image(5, 5, id(weather));
          if (id(weather_home).has_state()) {           
            it.printf(15, 75, id(my_font), st7735::ST7735_BLUE, "%s", id(weather_home).state.c_str());
          }
          if (id(home_temperature).has_state()) {  
            it.printf(15, 95, id(my_font), st7735::ST7735_BLUE, "%s C", id(home_temperature).state.c_str());
          }

interval:
  - interval: 5s
    then:
      - display.page.show_next: ha_display
      - component.update: ha_display
```
