#include "Arduino.h"

#include "Fw_global_config.h"

#include "eoss3_hal_adc.h"
#include "eoss3_hal_gpio.h"
#include "eoss3_hal_pads.h"
#include "eoss3_hal_pad_config.h"
#include "eoss3_hal_timer.h"


typedef struct
{
    bool hasGpioFunc;
    uint8_t gpio;
    uint32_t gpioFunc;
    uint32_t fpgaFunc;
} PadRegistry_t;

PadRegistry_t pad_registry[PAD_COUNT]= {
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD0_FUNC_SEL_FBIO_0,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD1_FUNC_SEL_FBIO_1,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD2_FUNC_SEL_FBIO_2,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD3_FUNC_SEL_FBIO_3,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD4_FUNC_SEL_FBIO_4,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD5_FUNC_SEL_FBIO_5,
    },
    {
        .hasGpioFunc = true,
        .fpgaFunc =  PAD6_FUNC_SEL_FBIO_6,
        .gpio = 0,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD7_FUNC_SEL_FBIO_7,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD8_FUNC_SEL_FBIO_8,
    },
    {
        .hasGpioFunc = true,
        .fpgaFunc =  PAD9_FUNC_SEL_FBIO_9,
        .gpio = 1,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD10_FUNC_SEL_FBIO_10,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD11_FUNC_SEL_FBIO_11,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD12_FUNC_SEL_FBIO_12,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD13_FUNC_SEL_FBIO_13,
    },
    {
        .hasGpioFunc = true,
        .fpgaFunc =  PAD14_FUNC_SEL_FBIO_14,
        .gpio = 3,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD15_FUNC_SEL_FBIO_15,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD16_FUNC_SEL_FBIO_16,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD17_FUNC_SEL_FBIO_17,
    },
    {
        .hasGpioFunc = true,
        .fpgaFunc =  PAD18_FUNC_SEL_FBIO_18,
        .gpio = 4,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD19_FUNC_SEL_FBIO_19,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD20_FUNC_SEL_FBIO_20,
    },
    {
        .hasGpioFunc = true,
        .fpgaFunc =  PAD21_FUNC_SEL_FBIO_21,
        .gpio = 5,
    },
    {
        .hasGpioFunc = true,
        .fpgaFunc =  PAD22_FUNC_SEL_FBIO_22,
        .gpio = 6,
    },
    {
        .hasGpioFunc = true,
        .fpgaFunc =  PAD23_FUNC_SEL_FBIO_23,
        .gpio = 7,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD24_FUNC_SEL_FBIO_24,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD25_FUNC_SEL_FBIO_25,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD26_FUNC_SEL_FBIO_26,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD27_FUNC_SEL_FBIO_27,
    },
    {
        .hasGpioFunc = true,
        .fpgaFunc =  PAD28_FUNC_SEL_FBIO_28,
        .gpio = 2,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD29_FUNC_SEL_FBIO_29,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD30_FUNC_SEL_FBIO_30,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD31_FUNC_SEL_FBIO_31,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD32_FUNC_SEL_FBIO_32,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD33_FUNC_SEL_FBIO_33,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD34_FUNC_SEL_FBIO_34,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD35_FUNC_SEL_FBIO_35,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD36_FUNC_SEL_FBIO_36,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD37_FUNC_SEL_FBIO_37,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD38_FUNC_SEL_FBIO_38,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD39_FUNC_SEL_FBIO_39,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD40_FUNC_SEL_FBIO_40,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD41_FUNC_SEL_FBIO_41,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD42_FUNC_SEL_FBIO_42,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD43_FUNC_SEL_FBIO_43,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD44_FUNC_SEL_FBIO_44,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD45_FUNC_SEL_FBIO_45,
    },
};

static bool pad_valid(int pin) {
    if((pin < 0) || (pin >= PAD_COUNT))
        return false;

    return true;
}

void digitalWrite(uint8_t pin, uint8_t value)
{
    if(!pad_valid(pin))
        return;

    if(!pad_registry[pin].hasGpioFunc)
        return;
    
    HAL_GPIO_Write(pad_registry[pin].gpio, (value == LOW) ? 0 : 1);
}

int digitalRead(uint8_t pin)
{
    if(!pad_valid(pin))
        return LOW;

    if(!pad_registry[pin].hasGpioFunc)
        return LOW;

    uint8_t value;
    HAL_GPIO_Read(pad_registry[pin].gpio, &value);
    return (value == 1 ? HIGH : LOW);
}

void pinMode(uint8_t pin, uint8_t mode)
{
    if(!pad_valid(pin))
        return;

    if((mode != INPUT) && (mode != INPUT_PULLUP) && (mode != INPUT_PULLDOWN) && (mode != FPGA))
        return;

    if(((mode == INPUT) || (mode == INPUT_PULLUP) || (mode == INPUT_PULLDOWN)) && pad_registry[pin].hasGpioFunc == false)
        return;

    // TODO: Does FPGA mode need pullups?
    const uint8_t ucPull = ((mode == INPUT_PULLUP) ? PAD_PULLUP :
			    (mode == INPUT_PULLDOWN) ? PAD_PULLDOWN :
                            PAD_NOPULL);

    const uint8_t ucCtrl = (((mode == INPUT) || (mode == INPUT_PULLUP) || (mode == INPUT_PULLDOWN)) ? PAD_CTRL_SRC_A0 :
                            (mode == FPGA) ? PAD_CTRL_SRC_FPGA :
                            PAD_CTRL_SRC_OTHER);

    const uint8_t ucMode = (((mode == INPUT) || (mode == INPUT_PULLUP) || (mode == INPUT_PULLDOWN)) ? PAD_MODE_INPUT_EN :
                            (mode == OUTPUT) ? PAD_MODE_OUTPUT_EN :
                            PAD_MODE_OUTPUT_EN);

    const uint32_t ucFunc = (((mode == INPUT) || (mode == INPUT_PULLUP) || (mode == INPUT_PULLDOWN)) ? pad_registry[pin].gpioFunc :
                            (mode == FPGA) ? pad_registry[pin].fpgaFunc :
                            0);

    // Note: Just using defaults for the drive strength, slew rate, and trigger; use the HAL directly for more functionality.
    PadConfig pad_config = {
	.ucPin = pin,                            /*!< Pad or Pin number starts from 0, Refer file eoss3_hal_pads.h macros PAD_0 to PAD_45*/
    .ucFunc = ucFunc,                        /*!< Pad function selection listed in eoss3_hal_pad.h for each pad, each pad function listed with PAD0_FUNC_XXX to PAD45_FUNC_XXX*/
    .ucCtrl = ucCtrl,                        /*!< Pad controller A0 = Firmware; Other=M4 HW(Debugger, I2C) or FFE, FPGA */
	.ucMode = ucMode,                        /*!< Pad Output or Input function, OEN or REN*/
	.ucPull = ucPull,                        /*!< Pad Pull up config Z/up/down/keeper*/
	.ucDrv = PAD_DRV_STRENGTH_4MA,           /*!< Pad Current driving strength*/
	.ucSpeed = PAD_SLEW_RATE_SLOW,           /*!< Pad operating speed slow or fast*/
	.ucSmtTrg = PAD_SMT_TRIG_DIS,            /*!< Pad configure for Schmitt trigger*/
    };

    
    HAL_PAD_Reconfig(&pad_config);
    return;
}

int analogRead(uint8_t pin) {
    ADC_channel channel;

    switch(pin) {
        case ADC0:
            channel = ADC_CHANNEL_0;
            break;
        case ADC1:
            channel = ADC_CHANNEL_1;
            break;
        default:
            return 0;
            break;
    }

    uint16_t measurement;

    // TODO: Only call this if we need to switch channels
    HAL_StatusTypeDef ret = HAL_ADC_Init(channel, 0);
    if(ret != HAL_OK)
       return 0; 

    HAL_ADC_StartConversion();

    ret = HAL_ADC_GetData(&measurement);
    if(ret != HAL_OK)
        return 0;

    return measurement;
}

void delay(unsigned long time) {
    // TODO: Handle full input range
    HAL_DelayUSec(time*1000);
}

void delayMicroseconds(unsigned int us) {
    HAL_DelayUSec(us);
}

#if 0
typedef void (*)(void) interrupt_function_t;

interrupt_function_t interrupt_functions[8] = {};

void attachInterrupt(uint8_t gpio, void (*)(void) callback, int mode) {
    interrupt_functions[gpio] = callback;
    set_gpio_interrupt_sensitivity(gpio);
    unmaks_gpio_interrupt(gpio);
}

void detachInterrupt(uint8_t gpio) {
    mask_gpio_interrupt(gpio);
    interrupt_functions[gpio] = NULL;
}
#endif

void    SensorGpio_Handler  (void) {
#if 0
    uint8_t source_gpio = xx;
    if(interrupt_functions[source_gpio] != NULL) {
        *interrupt_function[source_gpio]();
    }

    clear_interrupt();
#endif
}

// Reset to bootloader
void bootloader() {
    // reboot to bootloader
    PMU->MISC_POR_3 = REBOOT_CAUSE_FLASHING;
    NVIC_SystemReset();
}

