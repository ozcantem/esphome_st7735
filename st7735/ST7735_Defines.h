#pragma once

namespace esphome {
namespace st7735 {

static const uint16_t TFT_WIDTH = 160;
static const uint16_t TFT_HEIGHT = 128;

// Color definitions for backwards compatibility with old sketches
// use colour definitions like TFT_BLACK to make sketches more portable
static const uint16_t ST7735_BLACK       = 0x0000 ;      /*   0,   0,   0 */
static const uint16_t ST7735_NAVY        = 0x000F ;      /*   0,   0, 128 */
static const uint16_t ST7735_DARKGREEN   = 0x03E0 ;     /*   0, 128,   0 */
static const uint16_t ST7735_DARKCYAN    = 0x03EF ;     /*   0, 128, 128 */
static const uint16_t ST7735_MAROON      = 0x7800 ;     /* 128,   0,   0 */
static const uint16_t ST7735_PURPLE      = 0x780F ;     /* 128,   0, 128 */
static const uint16_t ST7735_OLIVE       = 0x7BE0 ;     /* 128, 128,   0 */
static const uint16_t ST7735_LIGHTGREY   = 0xC618 ;     /* 192, 192, 192 */
static const uint16_t ST7735_DARKGREY    = 0x7BEF ;     /* 128, 128, 128 */
static const uint16_t ST7735_BLUE        = 0x001F ;     /*   0,   0, 255 */
static const uint16_t ST7735_GREEN       = 0x07E0 ;     /*   0, 255,   0 */
static const uint16_t ST7735_CYAN        = 0x07FF ;     /*   0, 255, 255 */
static const uint16_t ST7735_RED         = 0xF800 ;     /* 255,   0,   0 */
static const uint16_t ST7735_MAGENTA     = 0xF81F ;     /* 255,   0, 255 */
static const uint16_t ST7735_YELLOW      = 0xFFE0 ;     /* 255, 255,   0 */
static const uint16_t ST7735_WHITE       = 0xFFFF ;     /* 255, 255, 255 */
static const uint16_t ST7735_ORANGE      = 0xFD20 ;     /* 255, 165,   0 */
static const uint16_t ST7735_GREENYELLOW = 0xAFE5 ;     /* 173, 255,  47 */
static const uint16_t ST7735_PINK        = 0xF81F ;

// Delay between some initialisation commands
static const uint8_t TFT_INIT_DELAY = 0x80 ;

// special signifier for command lists
static const uint8_t ST_CMD_DELAY = 0x80 ;   

// Generic commands used by TFT_eSPI.cpp
static const uint8_t TFT_NOP     = 0x00 ;
static const uint8_t TFT_SWRST   = 0x01 ;

static const uint8_t TFT_CASET   = 0x2A ;
static const uint8_t TFT_PASET   = 0x2B ;
static const uint8_t TFT_RAMWR   = 0x2C ;

static const uint8_t TFT_RAMRD   = 0x2E ;
static const uint8_t TFT_IDXRD   = 0x00 ;//0xDD // ILI9341 only, indexed control register read

static const uint8_t TFT_MADCTL  = 0x36 ;
static const uint8_t TFT_MAD_MY  = 0x80 ;
static const uint8_t TFT_MAD_MX  = 0x40 ;
static const uint8_t TFT_MAD_MV  = 0x20 ;
static const uint8_t TFT_MAD_ML  = 0x10 ;
static const uint8_t TFT_MAD_BGR = 0x08 ;
static const uint8_t TFT_MAD_MH  = 0x04 ;
static const uint8_t TFT_MAD_RGB = 0x00 ;

static const uint8_t TFT_INVOFF  = 0x20 ;
static const uint8_t TFT_INVON   = 0x21 ;

// ST7735 specific commands used in init
static const uint8_t ST7735_NOP     = 0x00 ;
static const uint8_t ST7735_SWRESET = 0x01 ;
static const uint8_t ST7735_RDDID   = 0x04 ;
static const uint8_t ST7735_RDDST   = 0x09 ;

static const uint8_t ST7735_SLPIN   = 0x10 ;
static const uint8_t ST7735_SLPOUT  = 0x11 ;
static const uint8_t ST7735_PTLON   = 0x12 ;
static const uint8_t ST7735_NORON   = 0x13 ;

static const uint8_t ST7735_INVOFF  = 0x20 ;
static const uint8_t ST7735_INVON   = 0x21 ;
static const uint8_t ST7735_DISPOFF = 0x28 ;
static const uint8_t ST7735_DISPON  = 0x29 ;
static const uint8_t ST7735_CASET   = 0x2A ;
static const uint8_t ST7735_PASET   = 0x2B ;// PASET
static const uint8_t ST7735_RAMWR   = 0x2C ;
static const uint8_t ST7735_RAMRD   = 0x2E ;

static const uint8_t ST7735_PTLAR   = 0x30 ;
static const uint8_t ST7735_COLMOD  = 0x3A ;
static const uint8_t ST7735_MADCTL  = 0x36 ;

static const uint8_t ST7735_FRMCTR1 = 0xB1 ;
static const uint8_t ST7735_FRMCTR2 = 0xB2 ;
static const uint8_t ST7735_FRMCTR3 = 0xB3 ;
static const uint8_t ST7735_INVCTR  = 0xB4 ;
static const uint8_t ST7735_DISSET5 = 0xB6 ;

static const uint8_t ST7735_PWCTR1  = 0xC0 ;
static const uint8_t ST7735_PWCTR2  = 0xC1 ;
static const uint8_t ST7735_PWCTR3  = 0xC2 ;
static const uint8_t ST7735_PWCTR4  = 0xC3 ;
static const uint8_t ST7735_PWCTR5  = 0xC4 ;
static const uint8_t ST7735_VMCTR1  = 0xC5 ;

static const uint8_t ST7735_RDID1   = 0xDA ;
static const uint8_t ST7735_RDID2   = 0xDB ;
static const uint8_t ST7735_RDID3   = 0xDC ;
static const uint8_t ST7735_RDID4   = 0xDD ;

static const uint8_t ST7735_PWCTR6  = 0xFC ;

static const uint8_t ST7735_GMCTRP1 = 0xE0 ;
static const uint8_t ST7735_GMCTRN1 = 0xE1 ;


}  // namespace st7735
}  // namespace esphome

