#ifndef _YINGJIAN_H
#define _YINGJIAN_H

#include "common.h"
#include "input.h"
#include "app_state.h"

void init_gpio(void);
void adc_init(void);
void pit_init(void);
void pwm_init(void);
void SPI_Init(uint8 spi);
void delay(int ms);
void delay_us(int us);
void gpio_btn_isr(void);
void pit_time(void);
void pit_time0(void);
void dac_init(void);
void playmusic(uint16 play);

#endif
