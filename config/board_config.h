/**
 * @file board_config.h
 * @brief 板级硬件配置 — 所有硬件引脚/参数/时钟的编译期固化
 *
 * 目标1: 编译期常量固化 — 硬件参数前置至编译阶段
 * 目标9: 组件化工程解耦 — 板级配置独立，支持多板型切换
 */
#ifndef CONFIG_BOARD_CONFIG_H
#define CONFIG_BOARD_CONFIG_H

#include "compiler.h"

/*============================================================================
 * MCU 时钟配置 — 编译期固化
 *============================================================================*/
#define BOARD_MCU_CORE_CLK_HZ   100000000UL  /* 100MHz */
#define BOARD_MCU_BUS_CLK_HZ    50000000UL   /* 50MHz */
#define BOARD_MCU_FLEX_CLK_HZ   50000000UL   /* 50MHz */

FW_STATIC_ASSERT(BOARD_MCU_CORE_CLK_HZ == 100000000UL, core_clock_mismatch);
FW_STATIC_ASSERT(BOARD_MCU_BUS_CLK_HZ  == 50000000UL,  bus_clock_mismatch);

/*============================================================================
 * LCD (LQ12864 OLED) 引脚配置
 *============================================================================*/
#define BOARD_LCD_SCL_PORT    PTD
#define BOARD_LCD_SCL_PIN     0
#define BOARD_LCD_SDA_PORT    PTD
#define BOARD_LCD_SDA_PIN     1
#define BOARD_LCD_RST_PORT    PTD
#define BOARD_LCD_RST_PIN     2
#define BOARD_LCD_DC_PORT     PTD
#define BOARD_LCD_DC_PIN      3

#define BOARD_LCD_WIDTH       128
#define BOARD_LCD_HEIGHT      64
#define BOARD_LCD_PAGES       (BOARD_LCD_HEIGHT / 8)

FW_STATIC_ASSERT(BOARD_LCD_PAGES * 8 == BOARD_LCD_HEIGHT, lcd_page_height_mismatch);

/*============================================================================
 * 摇杆 ADC 配置
 *============================================================================*/
#define BOARD_JOY_ADC         ADC0
#define BOARD_JOY_X_CH        DAD1   /* 左右 */
#define BOARD_JOY_Y_CH        DAD3   /* 上下 */
#define BOARD_JOY_THRESHOLD_LOW   1000   /* 低于此值视为方向触发 */
#define BOARD_JOY_THRESHOLD_HIGH  3000   /* 高于此值视为方向触发 */
#define BOARD_JOY_DEADZONE_LOW    BOARD_JOY_THRESHOLD_LOW
#define BOARD_JOY_DEADZONE_HIGH   BOARD_JOY_THRESHOLD_HIGH

/*============================================================================
 * 按键 GPIO 配置
 *============================================================================*/
#define BOARD_BTN_CONFIRM_PORT  PORTD
#define BOARD_BTN_CONFIRM_PIN   5
#define BOARD_BTN_CANCEL_PORT   PORTD
#define BOARD_BTN_CANCEL_PIN    6

/*============================================================================
 * SD卡 (SDHC) 配置
 *============================================================================*/
#define BOARD_SDHC_SECTOR_SIZE  512

/*============================================================================
 * 视频播放配置
 *============================================================================*/
#define BOARD_VIDEO_FPS         20
#define BOARD_VIDEO_FRAME_MS    (1000 / BOARD_VIDEO_FPS)
#define BOARD_VIDEO_PIT         PIT0

/* 视频分辨率配置 — 用union节省RAM */
#define BOARD_VIDEO_RES_86      86   /* Bad Apple 宽度 */
#define BOARD_VIDEO_RES_114     114  /* Garnidelia 宽度 */
#define BOARD_VIDEO_RES_MAX     BOARD_VIDEO_RES_114
#define BOARD_VIDEO_FRAME_SIZE  (BOARD_VIDEO_RES_MAX * BOARD_LCD_PAGES)

FW_STATIC_ASSERT(BOARD_VIDEO_FRAME_MS * BOARD_VIDEO_FPS == 1000, video_fps_mismatch);

/*============================================================================
 * 音频配置
 *============================================================================*/
#define BOARD_DAC               DAC0
#define BOARD_AUDIO_PIT         PIT2
#define BOARD_AUDIO_BUF_SIZE    1024
#define BOARD_AUDIO_VOL_DEFAULT 10

/*============================================================================
 * SPI配置 (VS1053)
 *============================================================================*/
#define BOARD_VS1053_SPI        SPI0
#define BOARD_VS1053_PCS        SPI_PCS0

/*============================================================================
 * UART调试串口配置
 *============================================================================*/
#define BOARD_DEBUG_UART        UART0
#define BOARD_DEBUG_BAUDRATE    115200

/*============================================================================
 * 调试LED配置
 *============================================================================*/
#define BOARD_LED_PORT          PTC
#define BOARD_LED_PIN           1

#endif /* CONFIG_BOARD_CONFIG_H */
