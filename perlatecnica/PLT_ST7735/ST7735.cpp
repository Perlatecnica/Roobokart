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
 
#include "mbed.h"
#include "ST7735.h"

#define INITR_GREENTAB 0x0
#define INITR_REDTAB   0x1
#define INITR_BLACKTAB   0x2
 
#define INITR_18GREENTAB    INITR_GREENTAB
#define INITR_18REDTAB      INITR_REDTAB
#define INITR_18BLACKTAB    INITR_BLACKTAB
#define INITR_144GREENTAB   0x1

#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT_144 128 // for 1.44" display
#define ST7735_TFTHEIGHT_18  160 // for 1.8" display
#define ST7735_TFTHEIGHT  160
 
#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09
 
#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13
 
#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E
 
#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36
 
#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6
 
#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5
 
#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD
 
#define ST7735_PWCTR6  0xFC
 
#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1
 
inline uint16_t swapcolor(uint16_t x) { 
  return (x << 11) | (x & 0x07E0) | (x >> 11);
}
 
// Constructor 
ST7735::ST7735(PinName mosi, PinName miso, PinName sck, PinName cs, PinName rs, PinName rst) 
: lcdPort(mosi, miso, sck), _cs(cs), _rs(rs), _reset(rst), Adafruit_GFX(ST7735_TFTWIDTH, ST7735_TFTHEIGHT)
{}
 
 
void ST7735::writecommand(uint8_t c)
{
    lcdPort.lock();
    _rs = 0;
    _cs = 0;
    lcdPort.write( c );
    _cs = 1;
    lcdPort.unlock();
}
 
 
void ST7735::writedata(uint8_t c)
{
    lcdPort.lock();
    _rs = 1;
    _cs = 0;
    lcdPort.write( c ); 
    _cs = 1;
    lcdPort.unlock();
}
 
 
// Rather than a bazillion writecommand() and writedata() calls, screen
// initialization commands and arguments are organized in these tables
// stored in PROGMEM.  The table may look bulky, but that's mostly the
// formatting -- storage-wise this is hundreds of bytes more compact
// than the equivalent code.  Companion function follows.
#define DELAY 0x80
static unsigned char
  Bcmd[] = {                  // Initialization commands for 7735B screens
    18,                       // 18 commands in list:
    ST7735_SWRESET,   DELAY,  //  1: Software reset, no args, w/delay
      50,                     //     50 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, no args, w/delay
      255,                    //     255 = 500 ms delay
    ST7735_COLMOD , 1+DELAY,  //  3: Set color mode, 1 arg + delay:
      0x05,                   //     16-bit color
      10,                     //     10 ms delay
    ST7735_FRMCTR1, 3+DELAY,  //  4: Frame rate control, 3 args + delay:
      0x00,                   //     fastest refresh
      0x06,                   //     6 lines front porch
      0x03,                   //     3 lines back porch
      10,                     //     10 ms delay
    ST7735_MADCTL , 1      ,  //  5: Memory access ctrl (directions), 1 arg:
      0x08,                   //     Row addr/col addr, bottom to top refresh
    ST7735_DISSET5, 2      ,  //  6: Display settings #5, 2 args, no delay:
      0x15,                   //     1 clk cycle nonoverlap, 2 cycle gate
                              //     rise, 3 cycle osc equalize
      0x02,                   //     Fix on VTL
    ST7735_INVCTR , 1      ,  //  7: Display inversion control, 1 arg:
      0x0,                    //     Line inversion
    ST7735_PWCTR1 , 2+DELAY,  //  8: Power control, 2 args + delay:
      0x02,                   //     GVDD = 4.7V
      0x70,                   //     1.0uA
      10,                     //     10 ms delay
    ST7735_PWCTR2 , 1      ,  //  9: Power control, 1 arg, no delay:
      0x05,                   //     VGH = 14.7V, VGL = -7.35V
    ST7735_PWCTR3 , 2      ,  // 10: Power control, 2 args, no delay:
      0x01,                   //     Opamp current small
      0x02,                   //     Boost frequency
    ST7735_VMCTR1 , 2+DELAY,  // 11: Power control, 2 args + delay:
      0x3C,                   //     VCOMH = 4V
      0x38,                   //     VCOML = -1.1V
      10,                     //     10 ms delay
    ST7735_PWCTR6 , 2      ,  // 12: Power control, 2 args, no delay:
      0x11, 0x15,
    ST7735_GMCTRP1,16      ,  // 13: Magical unicorn dust, 16 args, no delay:
      0x09, 0x16, 0x09, 0x20, //     (seriously though, not sure what
      0x21, 0x1B, 0x13, 0x19, //      these config values represent)
      0x17, 0x15, 0x1E, 0x2B,
      0x04, 0x05, 0x02, 0x0E,
    ST7735_GMCTRN1,16+DELAY,  // 14: Sparkles and rainbows, 16 args + delay:
      0x0B, 0x14, 0x08, 0x1E, //     (ditto)
      0x22, 0x1D, 0x18, 0x1E,
      0x1B, 0x1A, 0x24, 0x2B,
      0x06, 0x06, 0x02, 0x0F,
      10,                     //     10 ms delay
    ST7735_CASET  , 4      ,  // 15: Column addr set, 4 args, no delay:
      0x00, 0x02,             //     XSTART = 2
      0x00, 0x81,             //     XEND = 129
    ST7735_RASET  , 4      ,  // 16: Row addr set, 4 args, no delay:
      0x00, 0x02,             //     XSTART = 1
      0x00, 0x81,             //     XEND = 160
    ST7735_NORON  ,   DELAY,  // 17: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,   DELAY,  // 18: Main screen turn on, no args, w/delay
      255 },                  //     255 = 500 ms delay
 
  Rcmd1[] = {                 // Init for 7735R, part 1 (red or green tab)
    15,                       // 15 commands in list:
    ST7735_SWRESET,   DELAY,  //  1: Software reset, 0 args, w/delay
      150,                    //     150 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, 0 args, w/delay
      255,                    //     500 ms delay
    ST7735_FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
      0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
      0x01, 0x2C, 0x2D,       //     Dot inversion mode
      0x01, 0x2C, 0x2D,       //     Line inversion mode
    ST7735_INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
      0x07,                   //     No inversion
    ST7735_PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
      0xA2,
      0x02,                   //     -4.6V
      0x84,                   //     AUTO mode
    ST7735_PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
      0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
      0x0A,                   //     Opamp current small
      0x00,                   //     Boost frequency
    ST7735_PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
      0x8A,                   //     BCLK/2, Opamp current small & Medium low
      0x2A,  
    ST7735_PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
      0x8A, 0xEE,
    ST7735_VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
      0x0E,
    ST7735_INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
    ST7735_MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
      0xC8,                   //     row addr/col addr, bottom to top refresh
    ST7735_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
      0x05 },                 //     16-bit color
 
Scmd[] = {                  // Initialization commands for 7735S screens
    16,                       // 17 commands in list:
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, no args, w/delay
    120,                    //     255 = 500 ms delay
    ST7735_FRMCTR1, 3,      //  4: Frame rate control 1, 3 args
    0x05,                   //     fastest refresh
    0x3C,                   //     6 lines front porch
    0x3C,                   //     3 lines back porch
    ST7735_FRMCTR2, 3,
    0x05,
    0x3C,
    0x3C,
    ST7735_FRMCTR3, 6,
    0x05,
    0x3C,
    0x3C,
    0x05,
    0x3C,
    0x3C,
    ST7735_INVCTR , 1      ,  //  7: Display inversion control, 1 arg:
    0x03,                    //     Line inversion
    ST7735_PWCTR1 , 3,  //  8: Power control, 2 args + delay:
    0x28,                   //     GVDD = 4.7V
    0x08,                   //     1.0uA
    0x04,
    ST7735_PWCTR2 , 1      ,  //  9: Power control, 1 arg, no delay:
    0xC0,                   //     VGH = 14.7V, VGL = -7.35V
    ST7735_PWCTR3 , 2      ,  // 10: Power control, 2 args, no delay:
    0x0D,                   //     Opamp current small
    0x00,                   //     Boost frequency
    ST7735_PWCTR4, 2,
    0x8D,
    0x2A,
    ST7735_PWCTR5, 2,
    0x8D,
    0xEE,
    ST7735_VMCTR1 , 1,  // 11: Power control, 2 args + delay:
    0x1A,                   //     VCOMH = 4V
    ST7735_MADCTL , 1      ,  //  5: Memory access ctrl (directions), 1 arg:
    0xC0,                   //     Row addr/col addr, bottom to top refresh
    ST7735_GMCTRP1, 16,     // 13: Magical unicorn dust, 16 args, no delay:
    0x04, 0x22, 0x07, 0x0A, //     (seriously though, not sure what
    0x2E, 0x30, 0x25, 0x2A, //      these config values represent)
    0x28, 0x26, 0x2E, 0x3A,
    0x00, 0x01, 0x03, 0x13,
    ST7735_GMCTRN1, 16,     // 14: Sparkles and rainbows, 16 args + delay:
    0x04, 0x16, 0x06, 0x0D, //     (ditto)
    0x2D, 0x26, 0x23, 0x27,
    0x27, 0x25, 0x2D, 0x3B,
    0x00, 0x01, 0x04, 0x13,
    ST7735_COLMOD , 1,  //  3: Set color mode, 1 arg + delay:
    0x05,                   //     16-bit color
    ST7735_DISPON ,   DELAY,  // 18: Main screen turn on, no args, w/delay
    255 },    //     255 = 500 ms delay

  Rcmd2green[] = {            // Init for 7735R, part 2 (green tab only)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x02,             //     XSTART = 0
      0x00, 0x7F+0x02,        //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x01,             //     XSTART = 0
      0x00, 0x9F+0x01 },      //     XEND = 159
  Rcmd2red[] = {              // Init for 7735R, part 2 (red tab only)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F,             //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x9F },           //     XEND = 159
 
  Rcmd2green144[] = {              // Init for 7735R, part 2 (green 1.44 tab)
    2,                        //  2 commands in list:
    ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F,             //     XEND = 127
    ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
      0x00, 0x00,             //     XSTART = 0
      0x00, 0x7F },           //     XEND = 127
 
  Rcmd3[] = {                 // Init for 7735R, part 3 (red or green tab)
    4,                        //  4 commands in list:
    ST7735_GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
      0x02, 0x1c, 0x07, 0x12,
      0x37, 0x32, 0x29, 0x2d,
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16      , //  2: Sparkles and rainbows, 16 args, no delay:
      0x03, 0x1d, 0x07, 0x06,
      0x2E, 0x2C, 0x29, 0x2D,
      0x2E, 0x2E, 0x37, 0x3F,
      0x00, 0x00, 0x02, 0x10,
    ST7735_NORON  ,    DELAY, //  3: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,    DELAY, //  4: Main screen turn on, no args w/delay
      100 };                  //     100 ms delay
 
 
// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in byte array.
void ST7735::commandList(uint8_t *addr)
{
 
    uint8_t  numCommands, numArgs;
    uint16_t ms;
 
    numCommands = *addr++;   // Number of commands to follow
    while(numCommands--) {                 // For each command...
        writecommand(*addr++); //   Read, issue command
        numArgs  = *addr++;    //   Number of args to follow
        ms       = numArgs & DELAY;          //   If hibit set, delay follows args
        numArgs &= ~DELAY;                   //   Mask out delay bit
        while(numArgs--) {                   //   For each argument...
            writedata(*addr++);  //     Read, issue argument
        }
 
        if(ms) {
            ms = *addr++; // Read post-command delay time (ms)
            if(ms == 255) ms = 500;     // If 255, delay for 500 ms
            ThisThread::sleep_for(ms);
        }
    }
}
 
 
// Initialization code common to both 'B' and 'R' type displays
void ST7735::commonInit(uint8_t *cmdList)
{
 
    colstart  = rowstart = 0; // May be overridden in init func
 
    lcdPort.lock();
    _rs = 1;
    _cs = 1;

    // use default SPI format
    lcdPort.format(8,0);
    lcdPort.frequency(42000000);

    // toggle RST low to reset; CS low so it'll listen to us
    _cs = 0;
    lcdPort.unlock();

    if (_reset.is_connected())
    {
        _reset = 1;
        ThisThread::sleep_for(500);
        _reset = 0;
        ThisThread::sleep_for(500);
        _reset = 1;
        ThisThread::sleep_for(500);
    }

    if(cmdList) commandList(cmdList);
}
 
 
// Initialization for ST7735B screens
void ST7735::initB(void)
{
    commonInit(Bcmd);
}

void ST7735::initS(void)
{
    commonInit(Scmd);
}
 
 
// Initialization for ST7735R screens (green or red tabs)
void ST7735::initR(uint8_t options) {
  commonInit(Rcmd1);
  if(options == INITR_GREENTAB) {
    commandList(Rcmd2green);
    colstart = 2;
    rowstart = 1;
  } else if(options == INITR_144GREENTAB) {
    _height = ST7735_TFTHEIGHT_144;
    commandList(Rcmd2green144);
    colstart = 2;
    rowstart = 3;
  } else {
    // colstart, rowstart left at default '0' values
    commandList(Rcmd2red);
  }
  commandList(Rcmd3);
 
  // if black, change MADCTL color filter
  if (options == INITR_BLACKTAB) {
    writecommand(ST7735_MADCTL);
    writedata(0xC0);
  }
 
  tabcolor = options;
}
 
void ST7735::setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1,
                                    uint8_t y1)
{
 
    writecommand(ST7735_CASET); // Column addr set
    writedata(0x00);
    writedata(x0+colstart);     // XSTART
    writedata(0x00);
    writedata(x1+colstart);     // XEND
 
    writecommand(ST7735_RASET); // Row addr set
    writedata(0x00);
    writedata(y0+rowstart);     // YSTART
    writedata(0x00);
    writedata(y1+rowstart);     // YEND
 
    writecommand(ST7735_RAMWR); // write to RAM
}
 
 
void ST7735::pushColor(uint16_t color)
{
    lcdPort.lock();
    _rs = 1;
    _cs = 0; 
    lcdPort.write( color >> 8 );
    lcdPort.write( color );
    _cs = 1;
    lcdPort.unlock();
}
 
 
void ST7735::drawPixel(int16_t x, int16_t y, uint16_t color)
{
 
    if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;
 
    setAddrWindow(x,y,x+1,y+1);
 
    lcdPort.lock();
    _rs = 1;
    _cs = 0; 
    lcdPort.write( color >> 8 );
    lcdPort.write( color ); 
    _cs = 1;
    lcdPort.unlock();
}
 
 
void ST7735::drawFastVLine(int16_t x, int16_t y, int16_t h,
                                    uint16_t color)
{
 
    // Rudimentary clipping
    if((x >= _width) || (y >= _height)) return;
    if((y+h-1) >= _height) h = _height-y;
    setAddrWindow(x, y, x, y+h-1);
 
    uint8_t hi = color >> 8, lo = color;
    lcdPort.lock();
    _rs = 1;
    _cs = 0;
    while (h--) {
        lcdPort.write( hi );
        lcdPort.write( lo );
    }
    _cs = 1;
    lcdPort.unlock();
}
 
 
void ST7735::drawFastHLine(int16_t x, int16_t y, int16_t w,
                                    uint16_t color)
{
 
    // Rudimentary clipping
    if((x >= _width) || (y >= _height)) return;
    if((x+w-1) >= _width)  w = _width-x;
    setAddrWindow(x, y, x+w-1, y);
 
    uint8_t hi = color >> 8, lo = color;
    lcdPort.lock();
    _rs = 1;
    _cs = 0;
    while (w--) {
        lcdPort.write( hi );
        lcdPort.write( lo );
    }
    _cs = 1;
    lcdPort.unlock();
}
 
 
 
void ST7735::fillScreen(uint16_t color)
{
    fillRect(0, 0,  _width, _height, color);
}
 
 
// fill a rectangle
void ST7735::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                               uint16_t color)
{
 
    // rudimentary clipping (drawChar w/big text requires this)
    if((x >= _width) || (y >= _height)) return;
    if((x + w - 1) >= _width)  w = _width  - x;
    if((y + h - 1) >= _height) h = _height - y;
 
    setAddrWindow(x, y, x+w-1, y+h-1);
 
    uint8_t hi = color >> 8, lo = color;
    lcdPort.lock();
    _rs = 1;
    _cs = 0;
    for(y=h; y>0; y--) {
        for(x=w; x>0; x--) {
            lcdPort.write( hi );
            lcdPort.write( lo );
        }
    }
 
    _cs = 1;
    lcdPort.unlock();
}
 
 
// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t ST7735::Color565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
 
 
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04
 
void ST7735::setRotation(uint8_t m) {
 
  writecommand(ST7735_MADCTL);
  rotation = m % 4; // can't be higher than 3
  switch (rotation) {
   case 0:
     if (tabcolor == INITR_BLACKTAB) {
       writedata(MADCTL_MX | MADCTL_MY | MADCTL_RGB);
     } else {
       writedata(MADCTL_MX | MADCTL_MY | MADCTL_BGR);
     }
     _width  = ST7735_TFTWIDTH;
 
     if (tabcolor == INITR_144GREENTAB) 
       _height = ST7735_TFTHEIGHT_144;
     else
       _height = ST7735_TFTHEIGHT_18;
 
     break;
   case 1:
     if (tabcolor == INITR_BLACKTAB) {
       writedata(MADCTL_MY | MADCTL_MV | MADCTL_RGB);
     } else {
       writedata(MADCTL_MY | MADCTL_MV | MADCTL_BGR);
     }
 
     if (tabcolor == INITR_144GREENTAB) 
       _width = ST7735_TFTHEIGHT_144;
     else
       _width = ST7735_TFTHEIGHT_18;
 
     _height = ST7735_TFTWIDTH;
     break;
  case 2:
     if (tabcolor == INITR_BLACKTAB) {
       writedata(MADCTL_RGB);
     } else {
       writedata(MADCTL_BGR);
     }
     _width  = ST7735_TFTWIDTH;
     if (tabcolor == INITR_144GREENTAB) 
       _height = ST7735_TFTHEIGHT_144;
     else
       _height = ST7735_TFTHEIGHT_18;
 
    break;
   case 3:
     if (tabcolor == INITR_BLACKTAB) {
       writedata(MADCTL_MX | MADCTL_MV | MADCTL_RGB);
     } else {
       writedata(MADCTL_MX | MADCTL_MV | MADCTL_BGR);
     }
     if (tabcolor == INITR_144GREENTAB) 
       _width = ST7735_TFTHEIGHT_144;
     else
       _width = ST7735_TFTHEIGHT_18;
 
     _height = ST7735_TFTWIDTH;
     break;
  }
} 
 
void ST7735::invertDisplay(bool i)
{
    writecommand(i ? ST7735_INVON : ST7735_INVOFF);
}
