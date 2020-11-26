#include "Fw_global_config.h"
#include "Bootconfig.h"

#include <stdio.h>

#include "eoss3_hal_timer.h"
#include "eoss3_hal_pad_config.h"

#include "dbg_uart.h"

#include "Arduino.h"

extern void qf_hardwareSetup();

void setup();
void loop();

int main(void)
{
    qf_hardwareSetup();

    // To initialize system, this interrupt should be triggered at main.
    // So, we will set its priority just before calling vTaskStartScheduler(), not the time of enabling each irq.
    NVIC_SetPriority(Ffe0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(SpiMs_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(CfgDma_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(Uart_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    NVIC_SetPriority(FbMsg_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);

    // For using the delay() function
    HAL_Delay_Init();

    setup();
    while(true) { loop(); };
}

//needed for startup_EOSS3b.s asm file
void SystemInit(void)
{

}


void setup(void)
{
    char buf[30];

    snprintf(buf,sizeof(buf),"ADC0: %i\n", analogRead(ADC0));
    dbg_str(buf);

    snprintf(buf,sizeof(buf),"ADC1: %i\n", analogRead(ADC1));
    dbg_str(buf);
}

void loop(void)
{
    digitalWrite(22, LOW);
    delay(500);
    digitalWrite(22, HIGH);
    delay(500);

    if(digitalRead(18) == LOW)
        bootloader();
}
