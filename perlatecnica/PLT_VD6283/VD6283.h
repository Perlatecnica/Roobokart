#ifndef PLT_VD6283_H
#define PLT_VD6283_H
 
#include "mbed.h"

#include "STALS_platform.h"
#include "STALS.h"
#include <cstdint>

#define UNUSED_P(x)				x __attribute__((unused))

#define VD6281_DEFAULT_I2C_ADDR			0x20
#define VD_ALL_CHANNELS				((1 << STALS_ALS_MAX_CHANNELS) - 1)
#define VD_AC_CHANNELS				(VD_ALL_CHANNELS & ~STALS_CHANNEL_6)

#define VD_CUT_INVALID_INDEX			(-1)
#define VD_CUT_2_0_INDEX			0
#define VD_CUT_2_1_INDEX			1
#define VD_CUT_2_2_INDEX			2
#define VD_CUT_2_3_INDEX			3

#define VD_CCT_MATRIX_STD_BOM			0
#define VD_CCT_MATRIX_2D_BOM			1
#define VD_CCT_MATRIX_2D_BOM_WITH_DIFFUSER	2
#define VD_CCT_MATRIX_6D_BOM			3
#define VD_CCT_MATRIX_6D_BOM_WITH_DIFFUSER	4
#define VD_CCT_MATRIX_V5_BOM			5
#define VD_CCT_MATRIX_V5_BOM_WITH_DIFFUSER	6

// cpp define

#define ARRAY_SIZE(a)   (sizeof(a) / sizeof(a[0]))

#define VERSION_MAJOR       2
#define VERSION_MINOR       2
#define VERSION_REVISION      5

#define COEFF_NB            4

#define VD6281_DEVICE       0x70

class VD6283 
{
    private:
    void *hdl;	
	struct STALS_Als_t meas;	
	DevI2C *client;	
    uint32_t current_exposure;
    uint16_t current_gain;

    public:
    VD6283() = delete;
    VD6283(DevI2C * dev) : client(dev) {}
    ~VD6283() { STALS_Term(hdl); }

    VD6283(const VD6283 &) = delete;
    VD6283(VD6283 &&) = delete;

    VD6283& operator=(const VD6283 &) = delete;
    VD6283& operator=(VD6283 &&) = delete;

    inline uint32_t getRed(bool raw = false) { return getColor(0, raw); }    
    inline uint32_t getVisible(bool raw = false) { return getColor(1, raw); }
    inline uint32_t getBlue(bool raw = false) { return getColor(2, raw); }
    inline uint32_t getGreen(bool raw = false) { return getColor(3, raw); }
    inline uint32_t getIR(bool raw = false) { return getColor(4, raw); }
    inline uint32_t getClear(bool raw = false) { return getColor(5 , raw); }
    
    uint32_t getColor(uint8_t channel, bool is_raw_enable)
    {
        return is_raw_enable ? (meas.CountValueRaw[channel] >> 8) : (meas.CountValue[channel] >> 8);
    }

    STALS_ErrCode_t init()
    {
        STALS_ErrCode_t error;
        error = STALS_Init("VD6283", client, VD6281_DEFAULT_I2C_ADDR, &hdl);
        if (error) return error;
    
        error = STALS_SetExposureTime(hdl, 100000, &current_exposure);
        if (error) return error;

        for(int c = 0; c < STALS_ALS_MAX_CHANNELS; c++) {
            error = STALS_SetGain(hdl, (STALS_Channel_Id_t)(1 << c), 0x0100, &current_gain);
            if (error) return error;
        }
        return error;
    }
    uint32_t get_exposure()
    {
        uint32_t current_exposure;
        STALS_GetExposureTime(hdl, &current_exposure);
        return current_exposure;
    }
    void setExposure(uint32_t next_exposure)
    {
        uint32_t current_exposure;
        STALS_SetExposureTime(hdl, next_exposure, &current_exposure);
    }
    void decrease_exposure() { setExposure(get_exposure() >> 1); }
    void increase_exposure() { setExposure(get_exposure() << 1); }
    
    void decrease_gain(uint8_t channels = VD_AC_CHANNELS)
    {
        int c;        
        for(c = 0; c < STALS_ALS_MAX_CHANNELS; c++) {
            if (channels & (1 << c))
            change_gain_channel((STALS_Channel_Id_t)(1<< c), get_gain_channel((STALS_Channel_Id_t)(1 << c)) >> 1);
        }
    }    
    void increase_gain(uint8_t channels = VD_AC_CHANNELS)
    {
        int c;
        for(c = 0; c < STALS_ALS_MAX_CHANNELS; c++) {
            if (channels & (1 << c))
            change_gain_channel((STALS_Channel_Id_t)(1<< c), get_gain_channel((STALS_Channel_Id_t)(1 << c)) << 1);
        }
    }
    uint16_t get_gain_channel(enum STALS_Channel_Id_t channel)
    {
        uint16_t gain;

        STALS_GetGain(hdl, channel, &gain);

        return gain;
    }
    void change_gain_channel(enum STALS_Channel_Id_t channel, uint16_t next_gain)
    {
        uint16_t gain;
        STALS_SetGain(hdl, channel, next_gain, &gain);        
    }

    void setGain(uint32_t pGain)
    {
        STALS_ErrCode_t error;
        for(int c = 0; c < STALS_ALS_MAX_CHANNELS; c++) {
            error = STALS_SetGain(hdl, (STALS_Channel_Id_t)(1 << c), 0x0100, &current_gain);
            if (error) return;
        }
    }
    
    void toggle_dark_output()
    {
        uint32_t value;
        STALS_GetControl(hdl, STALS_OUTPUT_DARK_ENABLE, &value);
        STALS_SetControl(hdl, STALS_OUTPUT_DARK_ENABLE, 1 - value);
    }
    void updateValues()
    {
        uint8_t is_valid;      
        STALS_Start(hdl, STALS_MODE_ALS_SINGLE_SHOT, VD_ALL_CHANNELS);
        do 
        {
            STALS_GetAlsValues(hdl, VD_ALL_CHANNELS, &meas, &is_valid);
        }
        while(!is_valid);        
        STALS_Stop(hdl, STALS_MODE_ALS_SINGLE_SHOT);
    }
    
};
 
#endif