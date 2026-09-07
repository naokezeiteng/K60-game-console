#include "common.h"
#include "yingjian.h"
#include "board_config.h"

GPIO_InitTypeDef gpio_init_struct;
ADC_InitTypeDef adc_init_struct;
PIT_InitTypeDef pit0_init_struct;
PIT_InitTypeDef pit1_init_struct;
FTM_InitTypeDef ftm_init_struct;
DAC_InitTypeDef dac_init_struct;

void playmusic(uint16 play)
{
    input_beep(play);
}

void init_gpio(void)
{
    gpio_init_struct.GPIO_PTx = BOARD_LCD_SCL_PORT;
    gpio_init_struct.GPIO_Pins = (1u << BOARD_LCD_SCL_PIN) | (1u << BOARD_LCD_SDA_PIN)
                               | (1u << BOARD_LCD_RST_PIN) | (1u << BOARD_LCD_DC_PIN);
    gpio_init_struct.GPIO_Dir = DIR_OUTPUT;
    gpio_init_struct.GPIO_Output = OUTPUT_H;
    gpio_init_struct.GPIO_PinControl = IRQC_DIS;
    LPLD_GPIO_Init(gpio_init_struct);

    gpio_init_struct.GPIO_PTx = PTD;
    gpio_init_struct.GPIO_Pins = GPIO_Pin4;
    gpio_init_struct.GPIO_Dir = DIR_OUTPUT;
    gpio_init_struct.GPIO_Output = OUTPUT_L;
    gpio_init_struct.GPIO_PinControl = IRQC_DIS;
    LPLD_GPIO_Init(gpio_init_struct);

    gpio_init_struct.GPIO_PTx = PTD;
    gpio_init_struct.GPIO_Pins = (1u << BOARD_BTN_CONFIRM_PIN) | (1u << BOARD_BTN_CANCEL_PIN);
    gpio_init_struct.GPIO_Dir = DIR_INPUT;
    gpio_init_struct.GPIO_PinControl = INPUT_PULL_UP | IRQC_FA;
    gpio_init_struct.GPIO_Isr = gpio_btn_isr;
    LPLD_GPIO_Init(gpio_init_struct);
    LPLD_GPIO_EnableIrq(gpio_init_struct);
}

void adc_init(void)
{
    adc_init_struct.ADC_Adcx = BOARD_JOY_ADC;
    adc_init_struct.ADC_DiffMode = ADC_SE;
    adc_init_struct.ADC_BitMode = SE_12BIT;
    adc_init_struct.ADC_SampleTimeCfg = SAMTIME_SHORT;
    adc_init_struct.ADC_HwAvgSel = HW_4AVG;
    adc_init_struct.ADC_CalEnable = TRUE;
    LPLD_ADC_Init(adc_init_struct);
    LPLD_ADC_Chn_Enable(BOARD_JOY_ADC, BOARD_JOY_X_CH);
    LPLD_ADC_Chn_Enable(BOARD_JOY_ADC, BOARD_JOY_Y_CH);

    adc_init_struct.ADC_Adcx = ADC1;
    adc_init_struct.ADC_DiffMode = ADC_SE;
    adc_init_struct.ADC_BitMode = SE_12BIT;
    adc_init_struct.ADC_SampleTimeCfg = SAMTIME_SHORT;
    adc_init_struct.ADC_HwAvgSel = HW_4AVG;
    adc_init_struct.ADC_CalEnable = TRUE;
    LPLD_ADC_Init(adc_init_struct);
    LPLD_ADC_Chn_Enable(ADC1, DAD1);
}

void pit_init(void)
{
    pit0_init_struct.PIT_Pitx = BOARD_VIDEO_PIT;
    pit0_init_struct.PIT_PeriodMs = BOARD_VIDEO_FRAME_MS;
    pit0_init_struct.PIT_Isr = pit_time0;
    LPLD_PIT_Init(pit0_init_struct);
    LPLD_PIT_EnableIrq(pit0_init_struct);

    pit1_init_struct.PIT_Pitx = PIT1;
    pit1_init_struct.PIT_PeriodMs = 10;
    pit1_init_struct.PIT_Isr = pit_time;
    LPLD_PIT_Init(pit1_init_struct);
    LPLD_PIT_EnableIrq(pit1_init_struct);
}

void pwm_init(void)
{
    ftm_init_struct.FTM_Ftmx = FTM0;
    ftm_init_struct.FTM_Mode = FTM_MODE_PWM;
    ftm_init_struct.FTM_PwmFreq = 50;
    LPLD_FTM_Init(ftm_init_struct);
    LPLD_FTM_PWM_Enable(FTM0, FTM_Ch0, 0, PTC1, ALIGN_LEFT);
}

void dac_init(void)
{
    dac_init_struct.DAC_Dacx = BOARD_DAC;
    LPLD_DAC_Init(dac_init_struct);
}

void SPI_Init(uint8 spi)
{
    SPI_InitTypeDef spi_init_param;
    spi_init_param.SPI_SPIx = BOARD_VS1053_SPI;
    spi_init_param.SPI_SckDivider = SPI_SCK_DIV_64;
    LPLD_SPI_Init(spi_init_param);
    if (spi == 1) {
        LPLD_SPI_EnableIrq(spi_init_param);
    } else {
        LPLD_SPI_Deinit(spi_init_param);
    }
}

void delay(int ms)
{
    if (ms < 1) {
        ms = 1;
    }
    LPLD_SYSTICK_DelayMs((uint32)ms);
}

void delay_us(int us)
{
    if (us < 1) {
        us = 1;
    }
    LPLD_SYSTICK_DelayUs((uint32)us);
}

void gpio_btn_isr(void)
{
    if (LPLD_GPIO_IsPinxExt(BOARD_BTN_CONFIRM_PORT, (1u << BOARD_BTN_CONFIRM_PIN))) {
        input_on_ok_isr();
    }
    if (LPLD_GPIO_IsPinxExt(BOARD_BTN_CANCEL_PORT, (1u << BOARD_BTN_CANCEL_PIN))) {
        input_on_back_isr();
    }
}

void pit_time0(void)
{
    input_video_isr();
}

void pit_time(void)
{
    input_tick_isr();
}
