#include "st7735.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace st7735 {

static const char *TAG = "st7735";

void st7735::setup_pins_() {
  this->init_internal_(this->get_buffer_length_());
  this->dc_pin_->setup();  // OUTPUT
  this->dc_pin_->digital_write(false);
  if (this->reset_pin_ != nullptr) {
    this->reset_pin_->setup();  // OUTPUT
    this->reset_pin_->digital_write(true);
  }
  if (this->led_pin_ != nullptr) {
    this->led_pin_->setup();
    this->led_pin_->digital_write(true);
  }
  this->spi_setup();
  this->reset_();
}

void st7735::dump_config() {
  LOG_DISPLAY("", "st7735", this);
  ESP_LOGCONFIG(TAG, "  Width: %d, Height: %d,  Rotation: %d", this->width_, this->height_, this->rotation_);
  LOG_PIN("  Reset Pin: ", this->reset_pin_);
  LOG_PIN("  DC Pin: ", this->dc_pin_);
  LOG_PIN("  Busy Pin: ", this->busy_pin_);
  LOG_PIN("  Backlight Pin: ", this->led_pin_);
  LOG_UPDATE_INTERVAL(this);
}

float st7735::get_setup_priority() const { return setup_priority::PROCESSOR; 
}

void st7735::command(uint8_t value) {
  this->start_command_();
  this->write_byte(value);
  this->end_command_();
}

void st7735::reset_() {
  if (this->reset_pin_ != nullptr) {
    this->reset_pin_->digital_write(false);
    delay(200);
    this->reset_pin_->digital_write(true);
    delay(200);
  }
}

void st7735::data(uint8_t value) {
  this->start_data_();
  this->write_byte(value);
  this->end_data_();
}

void st7735::send_command(uint8_t commandByte, const uint8_t *dataBytes, uint8_t numDataBytes) {
  this->command(commandByte);  // Send the command byte
  this->start_data_();
  for (int i = 0; i < numDataBytes; i++) {
    this->write_byte(*dataBytes);  // Send the data bytes
    dataBytes++;
  }
  this->end_data_();
}

uint8_t st7735::read_command_(uint8_t commandByte, uint8_t index) {
  uint8_t data = 0x10 + index;
  this->send_command(0xD9, &data, 1);  // Set Index Register
  uint8_t result;
  this->start_command_();
  this->write_byte(commandByte);
  this->start_data_();
  do {
    result = this->read_byte();
  } while (index--);
  this->end_data_();
  return result;
}

void st7735::update() {
  this->do_update_();
  this->display();
}

void st7735::display () {
  //we will only update the changed window to the display
  int w = this->x_high_ - this->x_low_;
  int h = this->y_high_ - this->y_low_;

  set_addr_window_(this->x_low_,this->y_low_,w, h);
  this->start_data_();
  uint32_t start_pos = ((this->y_low_ * this->width_) + x_low_);
  for (uint16_t row = 0; row < h; row++) {
    for(uint16_t col = 0; col < w; col++) {
      uint32_t pos = start_pos + (row * width_) + col;

      uint16_t color = convert_to_16bit_color_(buffer_[pos]);
      this->write_byte(color >> 8);
      this->write_byte(color);  
    }
  }
  this->end_data_();

  //invalidate watermarks
  this->x_low_ = this->width_;
  this->y_low_ = this->height_;
  this->x_high_ = 0;
  this->y_high_ = 0;
}

uint16_t st7735::convert_to_16bit_color_ (uint8_t color_8bit) {
    int r = color_8bit >> 5;
    int g = (color_8bit >> 2 )& 0x07;
    int b = color_8bit & 0x03;
    uint16_t color = (r * 0x04) << 11;
    color |= (g * 0x09) << 5;
    color |= (b * 0x0A);
    
    return color;
}

uint8_t st7735::convert_to_8bit_color (uint16_t color_16bit) {
  //convert 16bit color to 8 bit buffer
  uint8_t r = color_16bit >> 11;
  uint8_t g = (color_16bit >> 5 ) &  0x3F;
  uint8_t b = color_16bit & 0x1F;

  return((b / 0x0A) | ((g / 0x09) << 2) | ((r / 0x04) << 5));
}

/**
 * do nothing.
 * we need this function het to override the default behaviour.
 * Otherwise the buffer is cleared at every update
 * */
void st7735::fill(int color) {
  //do nothing.
}

void st7735::fill_internal_(int color) {
  this->set_addr_window_(0, 0, this->get_width_internal(), this->get_height_internal());
  this->start_data_();
  for (uint32_t i = 0; i < (this->get_width_internal()) * (this->get_height_internal()); i++) {
    this->write_byte(color >> 8);
    this->write_byte(color);
    buffer_[i] = 0;
  }
  this->end_data_();
}

void HOT st7735::draw_absolute_pixel_internal(int x, int y, int color) {
  if (x >= this->get_width_internal() || x < 0 || y >= this->get_height_internal() || y < 0)
    return;

  // low and high watermark may speed up drawing from buffer
  this->x_low_ = (x < this->x_low_) ? x : this->x_low_;
  this->y_low_ = (y < this->y_low_) ? y : this->y_low_;  
  this->x_high_ = (x > this->x_high_) ? x : this->x_high_;
  this->y_high_ = (y > this->y_high_) ? y : this->y_high_;

  uint32_t pos = (y*width_) + x;
  buffer_[pos] = convert_to_8bit_color(color);
}

// should return the total size: return this->get_width_internal() * this->get_height_internal() * 2 // 16bit color
// values per bit is huge
uint32_t st7735::get_buffer_length_() { return this->get_width_internal() * this->get_height_internal(); 
}

void st7735::start_command_() {
  this->dc_pin_->digital_write(false);
  this->enable();
}

void st7735::end_command_() { this->disable(); 
}

void st7735::start_data_() {
  this->dc_pin_->digital_write(true);
  this->enable();
}

void st7735::end_data_() { this->disable(); }

void st7735::init_lcd_(const uint8_t *addr) {
  uint8_t  numCommands, cmd, numArgs;
  uint16_t ms;

  numCommands = pgm_read_byte(addr++);   // Number of commands to follow
  while(numCommands--) {                 // For each command...
    cmd = pgm_read_byte(addr++);         // Read command
    numArgs  = pgm_read_byte(addr++);    // Number of args to follow
    ms       = numArgs & ST_CMD_DELAY;   // If hibit set, delay follows args
    numArgs &= ~ST_CMD_DELAY;            // Mask out delay bit
    send_command(cmd, addr, numArgs);
    addr += numArgs;

    if(ms) {
      ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
      if(ms == 255) ms = 500;     // If 255, delay for 500 ms
      delay(ms);
    }
  }
}

void st7735::common_init(const uint8_t *cmdList) {
  if(cmdList) {
    init_lcd_(cmdList);
  }
}

void st7735::set_addr_window_(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h) {
  uint16_t x2 = (x1 + w - 1), y2 = (y1 + h - 1);
  this->command(ST7735_CASET);  // Column address set
  this->start_data_();
  this->write_byte(x1 >> 8);
  this->write_byte(x1);
  this->write_byte(x2 >> 8);
  this->write_byte(x2);
  this->end_data_();
  this->command(ST7735_PASET);  // Row address set
  this->start_data_();
  this->write_byte(y1 >> 8);
  this->write_byte(y1);
  this->write_byte(y2 >> 8);
  this->write_byte(y2);
  this->end_data_();
  this->command(ST7735_RAMWR);  // Write to RAM
}

void st7735::invert_display_(bool invert) { this->command(invert ? ST7735_INVON : ST7735_INVOFF); 
}

int st7735::get_width_internal() { return this->width_; 
}

int st7735::get_height_internal() { return this->height_; 
}

//   18_TFT display
void st7735_18_TFT::initialize() {
  this->width_ = 128;
  this->height_ = 160;
  common_init(Rcmd1);
  // colstart, rowstart left at default '0' values
  init_lcd_(Rcmd2red);
  init_lcd_(Rcmd3);
  // Black tab, change MADCTL color filter
  uint8_t data = 0xC0;
  send_command(ST7735_MADCTL, &data, 1);
  this->fill_internal_(ST7735_BLACK);
  
}

}  // namespace ili9341
}  // namespace esphome
