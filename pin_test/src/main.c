/*==========================================================
 * Copyright 2020 QuickLogic Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *==========================================================*/

/*==========================================================
 *
 *    File   : main.c
 *    Purpose: main standalone FPGA applications
 *                                                          
 *=========================================================*/

#include "Fw_global_config.h"
#include "Bootconfig.h"

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "RtosTask.h"

/*    Include the generic headers required for sensorHub */
#include "eoss3_hal_gpio.h"
#include "eoss3_hal_rtc.h"
#include "eoss3_hal_pads.h"
#include "eoss3_hal_pad_config.h"

#include "ql_time.h"
#include "s3x_clock_hal.h"
#include "s3x_clock.h"
#include "s3x_pi.h"
#include "dbg_uart.h"
#include "eoss3_hal_spi.h"

#include "fpga_loader.h"        // API for loading FPGA
#include "helloworldfpga_bit.h"   // FPGA bitstream to load into FPGA


const char *SOFTWARE_VERSION_STR;


/*
 * Global variable definition
 */


extern void qf_hardwareSetup();
static void nvic_init(void);


typedef enum
{
    INPUT,
    INPUT_PULLUP,
    INPUT_PULLDOWN,
    OUTPUT,
    FPGA
} pinmode_t;

typedef struct
{
    bool hasGpioFunc;
    uint32_t gpioFunc;
    uint32_t fpgaFunc;
} PadRegistry_t;

PadRegistry_t pad_registry[]= {
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
        .hasGpioFunc = false,
        .fpgaFunc =  PAD6_FUNC_SEL_FBIO_6,
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
        .hasGpioFunc = false,
        .fpgaFunc =  PAD9_FUNC_SEL_FBIO_9,
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
        .hasGpioFunc = false,
        .fpgaFunc =  PAD14_FUNC_SEL_FBIO_14,
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
        .hasGpioFunc = false,
        .fpgaFunc =  PAD18_FUNC_SEL_FBIO_18,
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
        .hasGpioFunc = false,
        .fpgaFunc =  PAD21_FUNC_SEL_FBIO_21,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD22_FUNC_SEL_FBIO_22,
    },
    {
        .hasGpioFunc = false,
        .fpgaFunc =  PAD23_FUNC_SEL_FBIO_23,
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
        .hasGpioFunc = false,
        .fpgaFunc =  PAD28_FUNC_SEL_FBIO_28,
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

/*
#define HIGH 0x1
#define LOW 0x0

void digitalWrite(int pin, uint8_t value)
{
    HAL_GPIO_Write(pad_registry[pin], (value == LOW) ? 0 : 1);
}

uint8_t  digitalRead(int pin)
{
    uint8_t value;
    HAL_GPIO_Read(pad_registry[pin], &value);
    return value;
}
*/

bool pinMode(int pin, pinmode_t mode)
{
    if((pin < 0) || (pin > 45))
        return false;

    if((mode != INPUT) && (mode != INPUT_PULLUP) && (mode != INPUT_PULLDOWN) && (mode != FPGA))
        return false;

    if(((mode == INPUT) || (mode == INPUT_PULLUP) || (mode == INPUT_PULLDOWN)) && pad_registry[pin].hasGpioFunc == false)
        return false;

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
    return true;
}






int main(void)
{

    SOFTWARE_VERSION_STR = "qorc-sdk/qf_apps/qf_helloworldhw";
    
    qf_hardwareSetup();                                     // Note: pincfg_table.c has been updated to give FPGA control of LEDs
    nvic_init();
    S3x_Clk_Disable(S3X_FB_21_CLK);
    S3x_Clk_Disable(S3X_FB_16_CLK);
    S3x_Clk_Enable(S3X_A1_CLK);
    S3x_Clk_Enable(S3X_CFG_DMA_A1_CLK);
    load_fpga(sizeof(axFPGABitStream),axFPGABitStream);     // Load bitstrem into FPGA
    S3x_Clk_Enable(S3X_FB_21_CLK);                          // Start FPGA clock
    S3x_Clk_Enable(S3X_FB_16_CLK);
    
    dbg_str("\n\n");
    dbg_str( "##########################\n");
    dbg_str( "Quicklogic QuickFeather Standalone FPGA\n");
    dbg_str( "SW Version: ");
    dbg_str( SOFTWARE_VERSION_STR );
    dbg_str( "\n" );
    dbg_str( __DATE__ " " __TIME__ "\n" );
    dbg_str( "##########################\n\n");

    for(int pin = 0; pin < 46; pin++) {
        if(pin == 16) continue;	// UART RX
        if(pin == 24) continue;	// UART TX

        if(pin == 37) continue; // PU_CTRL_USBP
        if(pin == 39) continue; // SPI_MST_SS1
        if(pin == 41) continue; // S3_USB_DN
        if(pin == 42) continue; // S3_USB_DP

        const bool ret = pinMode(pin, FPGA);
        if(ret)
            dbg_str("OK\n");
        else
            dbg_str("FAIL\n");
    }

      
    while(1); // Let FPGA do its thing
}

static void nvic_init(void)
 {
    // To initialize system, this interrupt should be triggered at main.
    // So, we will set its priority just before calling vTaskStartScheduler(), not the time of enabling each irq.
    NVIC_SetPriority(Ffe0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SpiMs_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(CfgDma_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(Uart_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FbMsg_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
 }    

//needed for startup_EOSS3b.s asm file
void SystemInit(void)
{

}


