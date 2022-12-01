/*
    Made by Francesco Caiazzo
    Added multithread support
    Added multiple and differents spi devices support
    Added Mbed 6 support
    Added external reset support
    Added for ST7735S displays support

    Based on mbed repository:
    https://os.mbed.com/users/SomeRandomBloke/code/Adafruit_ST7735/
*/

/***************************************************
  This is a library for the Adafruit 1.8" SPI display.
  This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
  as well as Adafruit raw 1.8" TFT display
  ----> http://www.adafruit.com/products/618
 
  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
 
  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
 
#ifndef _ADAFRUIT_ST7735H_
#define _ADAFRUIT_ST7735H_
 
#include "mbed.h"
#include "Adafruit_GFX.h"
#include <cstdint>
 
//#define boolean bool
 
// some flags for initR() :(


 
// Color definitions

 
 
class ST7735 : public Adafruit_GFX 
{ 
    uint8_t tabcolor;
    uint8_t  colstart, rowstart; // some displays need this changed
    SPI lcdPort;            // does SPI MOSI, MISO and SCK
    DigitalOut _cs;         // does SPI CE
    DigitalOut _rs;         // register/date select
    DigitalOut _reset;        // does 3310 LCD_RST

public:
    typedef enum : uint8_t
    {
        GreenTab = 0x0,
        RedTab = 0x1,
        BlackTab = 0x2,        
        GreenTab18 = GreenTab,
        RedTab18 = RedTab,
        BlackTab18 = BlackTab,
        GreenTab144 = 0x1
    } InitR;
    
    typedef enum : uint16_t {
        Black   = 0x0000,
        Blue    = 0x001F,
        Red     = 0xF800,
        Green   = 0x07E0,
        Cyan    = 0x07FF,
        Magenta = 0xF81F,
        Yellow  = 0xFFE0,
        White   = 0xFFFF
    } Colors;
 
    ST7735(PinName mosi, PinName miso, PinName sck, PinName CS, PinName RS, PinName RST);

    void     initB(void);            // for ST7735B displays
    void     initS(void);            // for ST7735S displays
    void     initR(uint8_t options); // for ST7735R
    void     setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
    void     pushColor(uint16_t color);

    void     fillScreen(uint16_t color);
    void     drawPixel(int16_t x, int16_t y, uint16_t color);
    void     drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void     drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void     fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void     invertDisplay(bool i);

    void     setRotation(uint8_t r);
    uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);

    private:
    void spiwrite(uint8_t);
    void writecommand(uint8_t c);
    void writedata(uint8_t d);
    void commandList(uint8_t *addr);
    void commonInit(uint8_t *cmdList);
};
 
#endif
