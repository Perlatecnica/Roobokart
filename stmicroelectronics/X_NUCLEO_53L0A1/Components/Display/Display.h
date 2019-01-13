/**
 ******************************************************************************
 * @file    Display.h
 * @author  AST / EST
 * @version V0.0.1
 * @date    14-April-2015
 * @brief   Header file for display
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
*/

#ifndef __DISPLAY_H
#define __DISPLAY_H

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "Stmpe1600.h"
#include "DevI2C.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * GPIO monitor pin state register
 * 16 bit register LSB at lowest offset (little endian)
 */
#define GPMR    0x10
/**
 * STMPE1600 GPIO set pin state register
 * 16 bit register LSB at lowest offset (little endian)
 */
#define GPSR    0x12
/**
 * STMPE1600 GPIO set pin direction register
 * 16 bit register LSB at lowest offset
 */
#define GPDR    0x14


/**
 * cache the full set of expanded GPIO values to avoid i2c reading
 */
static union CurIOVal_u {
    uint8_t bytes[4];   /*!<  4 bytes array i/o view */
    uint32_t u32;       /*!<  single dword i/o view */
}
/** cache the extended IO values */
cur_io_val;

/**
 * lookup table for digit to bit position
 */
static int  display_bit_posn[4] = {0, 7, 16, 16 + 7};

/**
 * @defgroup XNUCLEO53L0A1_7Segment 7 segment display
 *
 * macro use for human readable segment building
 * @code
 *  --s0--
 *  s    s
 *  5    1
 *  --s6--
 *  s    s
 *  4    2
 *  --s3-- . s7 (dp)
 * @endcode
 *
 * @{
 */
/** decimal point bit mapping*  */
#define DP  (1 << 7)

/** sgement s0 bit mapping*/
#define S0 (1 << 3)
/** sgement s1 bit mapping*/
#define S1 (1 << 5)
/** sgement s2 bit mapping*/
#define S2 (1 << 6)
/** sgement s3 bit mapping*/
#define S3 (1 << 4)
/** sgement s4 bit mapping*/
#define S4 (1 << 0)
/** sgement s5 bit mapping*/
#define S5 (1 << 1)
/** sgement s6 bit mapping*/
#define S6 (1 << 2)

/**
 * build a character by defining the non lighted segment (not one and no DP)
 *
 * @param  ... literal sum and or combine of any macro to define any segment #S0 .. #S6
 *
 * example '9' is all segment on but S4
 * @code
 *   ['9']=           NOT_7_NO_DP(S4),
 * @endcode
 */
#define NOT_7_NO_DP( ... ) (uint8_t) ~( __VA_ARGS__ + DP )

/**
 * Ascii to 7 segment  lookup table
 *
 * Most common character are supported and follow http://www.twyman.org.uk/Fonts/
 * few extra special \@ ^~ ... etc are present for specific demo purpose
 */
#ifndef __cpluplus
/* refer to http://www.twyman.org.uk/Fonts/ */
static const uint8_t ascii_to_display_lut[256] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    [32] =           0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0,
    [42] =           NOT_7_NO_DP(),
    0, 0,
    [45] =           S6,
    0, 0,
    [48] =           NOT_7_NO_DP(S6),
    [49] =           S1 + S2,
    [50] =           S0 + S1 + S6 + S4 + S3,
    [51] =           NOT_7_NO_DP(S4 + S5),
    [52] =           S5 + S1 + S6 + S2,
    [53] =           NOT_7_NO_DP(S1 + S4),
    [54] =           NOT_7_NO_DP(S1),
    [55] =           S0 + S1 + S2,
    [56] =           NOT_7_NO_DP(0),
    [57] =           NOT_7_NO_DP(S4),
    0, 0, 0,
    [61] =           S3 + S6,
    0,
    [63] =           NOT_7_NO_DP(S5 + S3 + S2),
    [64] =           S0 + S3,
    [65] =           NOT_7_NO_DP(S3),
    [66] =           NOT_7_NO_DP(S0 + S1),    /* as b  */
    [67] =           S0 + S3 + S4 + S5, // same as [
    [68] =           S0 + S3 + S4 + S5, // same as [ DUMMY
    [69] =           NOT_7_NO_DP(S1 + S2),
    [70] =           S6 + S5 + S4 + S0,
    [71] =           NOT_7_NO_DP(S4),    /* same as 9 */
    [72] =           NOT_7_NO_DP(S0 + S3),
    [73] =           S1 + S2,
    [74] =           S1 + S2 + S3 + S4,
    [75] =           NOT_7_NO_DP(S0 + S3),    /* same as H */
    [76] =           S3 + S4 + S5,
    [77] =           S0 + S4 + S2, /* same as  m*/
    [78] =           S2 + S4 + S6, /* same as n*/
    [79] =           NOT_7_NO_DP(S6),
    [80] =           S0 + S1 + S2 + S5 + S6, // sames as 'q'
    [81] =           NOT_7_NO_DP(S3 + S2),
    [82] =           S4 + S6,
    [83] =           NOT_7_NO_DP(S1 + S4),    /* sasme as 5 */
    [84] =           NOT_7_NO_DP(S0 + S1 + S2),    /* sasme as t */
    [85] =           NOT_7_NO_DP(S6 + S0),
    [86] =           S4 + S3 + S2, // is u but u use U
    [87] =           S1 + S3 + S5,
    [88] =           NOT_7_NO_DP(S0 + S3),    // similar to H
    [89] =           NOT_7_NO_DP(S0 + S4),
    [90] =           S0 + S1 + S6 + S4 + S3, // same as 2
    [91] =           S0 + S3 + S4 + S5,
    0,
    [93] =           S0 + S3 + S2 + S1,
    [94] =           S0, /* use as top bar */
    [95] =           S3,
    0,
    [97] =           S2 + S3 + S4 + S6,
    [98] =           NOT_7_NO_DP(S0 + S1),
    [99] =           S6 + S4 + S3,
    [100] =           NOT_7_NO_DP(S0 + S5),
    [101] =           NOT_7_NO_DP(S2),
    [102] =           S6 + S5 + S4 + S0, /* same as F */
    [103] =           NOT_7_NO_DP(S4),    /* same as 9 */
    [104] =           S6 + S5 + S4 + S2,
    [105] =           S4,
    [106] =           S1 + S2 + S3 + S4,
    [107] =           S6 + S5 + S4 + S2, /* a h */
    [108] =           S3 + S4,
    [109] =           S0 + S4 + S2, /* same as  */
    [110] =           S2 + S4 + S6,
    [111] =           S6 + S4 + S3 + S2,
    [112] =           NOT_7_NO_DP(S3 + S2),    // same as P
    [113] =           S0 + S1 + S2 + S5 + S6,
    [114] =           S4 + S6,
    [115] =           NOT_7_NO_DP(S1 + S4),
    [116] =           NOT_7_NO_DP(S0 + S1 + S2),
    [117] =           S4 + S3 + S2 + S5 + S1, // U
    [118] =           S4 + S3 + S2, // is u but u use U
    [119] =           S1 + S3 + S5,
    [120] =           NOT_7_NO_DP(S0 + S3),    // similar to H
    [121] =           NOT_7_NO_DP(S0 + S4),
    [122] =           S0 + S1 + S6 + S4 + S3, // same as 2
    0, 0, 0,
    [126] =           S0 + S3 + S6 /* 3 h bar */
};
#else
/* refer to http://www.twyman.org.uk/Fonts/ */
static const uint8_t ascii_to_display_lut[256] = {
    [' '] =           0,
    ['-'] =           S6,
    ['_'] =           S3,
    ['='] =           S3 + S6,
    ['~'] =           S0 + S3 + S6, /* 3 h bar */
    ['^'] =           S0, /* use as top bar */

    ['?'] =           NOT_7_NO_DP(S5 + S3 + S2),
    ['*'] =           NOT_7_NO_DP(),
    ['['] =           S0 + S3 + S4 + S5,
    [']'] =           S0 + S3 + S2 + S1,
    ['@'] =           S0 + S3,

    ['0'] =           NOT_7_NO_DP(S6),
    ['1'] =           S1 + S2,
    ['2'] =           S0 + S1 + S6 + S4 + S3,
    ['3'] =           NOT_7_NO_DP(S4 + S5),
    ['4'] =           S5 + S1 + S6 + S2,
    ['5'] =           NOT_7_NO_DP(S1 + S4),
    ['6'] =           NOT_7_NO_DP(S1),
    ['7'] =           S0 + S1 + S2,
    ['8'] =           NOT_7_NO_DP(0),
    ['9'] =           NOT_7_NO_DP(S4),

    ['a'] =           S2 + S3 + S4 + S6,
    ['b'] =           NOT_7_NO_DP(S0 + S1),
    ['c'] =           S6 + S4 + S3,
    ['d'] =           NOT_7_NO_DP(S0 + S5),
    ['e'] =           NOT_7_NO_DP(S2),
    ['f'] =           S6 + S5 + S4 + S0, /* same as F */
    ['g'] =           NOT_7_NO_DP(S4),    /* same as 9 */
    ['h'] =           S6 + S5 + S4 + S2,
    ['i'] =           S4,
    ['j'] =           S1 + S2 + S3 + S4,
    ['k'] =           S6 + S5 + S4 + S2, /* a h */
    ['l'] =           S3 + S4,
    ['m'] =           S0 + S4 + S2, /* same as  */
    ['n'] =           S2 + S4 + S6,
    ['o'] =           S6 + S4 + S3 + S2,
    ['p'] =           NOT_7_NO_DP(S3 + S2),    // same as P
    ['q'] =           S0 + S1 + S2 + S5 + S6,
    ['r'] =           S4 + S6,
    ['s'] =           NOT_7_NO_DP(S1 + S4),
    ['t'] =           NOT_7_NO_DP(S0 + S1 + S2),
    ['u'] =           S4 + S3 + S2 + S5 + S1, // U
    ['v'] =           S4 + S3 + S2, // is u but u use U
    ['w'] =           S1 + S3 + S5,
    ['x'] =           NOT_7_NO_DP(S0 + S3),    // similar to H
    ['y'] =           NOT_7_NO_DP(S0 + S4),
    ['z'] =           S0 + S1 + S6 + S4 + S3, // same as 2

    ['A'] =           NOT_7_NO_DP(S3),
    ['B'] =           NOT_7_NO_DP(S0 + S1),    /* as b  */
    ['C'] =           S0 + S3 + S4 + S5, // same as [
    ['E'] =           NOT_7_NO_DP(S1 + S2),
    ['F'] =           S6 + S5 + S4 + S0,
    ['G'] =           NOT_7_NO_DP(S4),    /* same as 9 */
    ['H'] =           NOT_7_NO_DP(S0 + S3),
    ['I'] =           S1 + S2,
    ['J'] =           S1 + S2 + S3 + S4,
    ['K'] =           NOT_7_NO_DP(S0 + S3),    /* same as H */
    ['L'] =           S3 + S4 + S5,
    ['M'] =           S0 + S4 + S2, /* same as  m*/
    ['N'] =           S2 + S4 + S6, /* same as n*/
    ['O'] =           NOT_7_NO_DP(S6),
    ['P'] =           S0 + S1 + S2 + S5 + S6, // sames as 'q'
    ['Q'] =           NOT_7_NO_DP(S3 + S2),
    ['R'] =           S4 + S6,
    ['S'] =           NOT_7_NO_DP(S1 + S4),    /* sasme as 5 */
    ['T'] =           NOT_7_NO_DP(S0 + S1 + S2),    /* sasme as t */
    ['U'] =           NOT_7_NO_DP(S6 + S0),
    ['V'] =           S4 + S3 + S2, // is u but u use U
    ['W'] =           S1 + S3 + S5,
    ['X'] =           NOT_7_NO_DP(S0 + S3),    // similar to H
    ['Y'] =           NOT_7_NO_DP(S0 + S4),
    ['Z'] =           S0 + S1 + S6 + S4 + S3 // same as 2
};
#endif


#undef S0
#undef S1
#undef S2
#undef S3
#undef S4
#undef S5
#undef S6
#undef DP

/** @} */

//#define DISPLAY_DELAY   1  // in mSec

/* Classes -------------------------------------------------------------------*/
/** Class representing Display
 */

class Display
{
private:
    Stmpe1600 &stmpe1600_exp0;
    Stmpe1600 &stmpe1600_exp1;
public:
    /** Constructor
    * @param[in] &stmpe_1600 device handler to be used for display control
    */
    Display(Stmpe1600 &stmpe_1600_exp0, Stmpe1600 &stmpe_1600_exp1) : stmpe1600_exp0(stmpe_1600_exp0),
        stmpe1600_exp1(stmpe_1600_exp1)
    {
        uint16_t expander_data;

        // detect the extenders
        stmpe1600_exp0.read_16bit_reg(0x00, &expander_data);
//        if (ExpanderData != 0x1600) {/* log - failed to find expander exp0 */ }
        stmpe1600_exp1.read_16bit_reg(0x00, &expander_data);
//        if (ExpanderData != 0x1600) {/* log - failed to find expander exp1 */ }

        // configure all necessary GPIO pins as outputs
        expander_data = 0xFFFF;
        stmpe1600_exp0.write_16bit_reg(GPDR, &expander_data);
        expander_data = 0xBFFF; // leave bit 14 as an input, for the pushbutton, PB1.
        stmpe1600_exp1.write_16bit_reg(GPDR, &expander_data);

        // shut down all segment and all device
        expander_data = 0x7F + (0x7F << 7);
        stmpe1600_exp0.write_16bit_reg(GPSR, &expander_data);
        stmpe1600_exp1.write_16bit_reg(GPSR, &expander_data);
    }

    /*** Interface Methods ***/
    /**
     * @brief       Print the string on display
     * @param[in]   String to be printed
     * @param[in]   String lenght [min 1, max 4]
     * @return      void
     */
    void display_string(const char *str)
    {
        uint16_t expander_data;
        uint32_t segments;
        int bit_posn;
        int i;

        for (i = 0; (i < 4 && str[i] != 0); i++) {
            segments = (uint32_t) ascii_to_display_lut[(uint8_t) str[i]];
            segments = (~segments) & 0x7F;
            bit_posn = display_bit_posn[i];
            cur_io_val.u32 &= ~(0x7F << bit_posn);
            cur_io_val.u32 |= segments << bit_posn;
        }
        /* clear unused digit */
        for (; i < 4; i++) {
            bit_posn = display_bit_posn[i];
            cur_io_val.u32 |= 0x7F << bit_posn;
        }

//        stmpe1600_exp0.write16bitReg(GPSR, (uint16_t *)&CurIOVal.bytes[0]);
//        stmpe1600_exp1.write16bitReg(GPSR, (uint16_t *)&CurIOVal.bytes[2]);

// ordered low-byte/high-byte!
        cur_io_val.bytes[1] |= 0xC0; // ensure highest bits are high, as these are xshutdown pins for left & right sensors!
        expander_data = (cur_io_val.bytes[1] << 8) + cur_io_val.bytes[0];
        stmpe1600_exp0.write_16bit_reg(GPSR, &expander_data);
        cur_io_val.bytes[3] |= 0x80; // ensure highest bit is high, as this is xshutdown pin on central sensor!
        expander_data = (cur_io_val.bytes[3] << 8) + cur_io_val.bytes[2];
        stmpe1600_exp1.write_16bit_reg(GPSR, &expander_data);

    }

    void clear_display(void)
    {
        uint16_t expander_data;

        expander_data = 0x7F + (0x7F << 7);
        stmpe1600_exp0.write_16bit_reg(GPSR, &expander_data);
        stmpe1600_exp1.write_16bit_reg(GPSR, &expander_data);
    }

};

#ifdef __cplusplus
}
#endif
#endif // __DISPLAY_H
