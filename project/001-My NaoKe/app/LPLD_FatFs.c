#include "common.h"
#include "LQ12864.h"
#include "yingjian.h"
#include "jianmian.h"
#include "tuixiangzi.h"
#include "saolei.h"
#include "wuziqi.h"
#include "huatu.h"

void start_pic(void);
void die(FRESULT rc);
DWORD get_fattime(void);
void badapple(uint8 spshu);
void flash_start(void);

FRESULT rc;
FATFS fatfs, *fs;
FIL fil;
UINT bw, br;

/* 两种分辨率不会同时使用，用 union 节省 RAM */
union {
    unsigned char b86[688];
    unsigned char b114[912];
} videobuf;
#define buff_86  videobuf.b86
#define buff_114 videobuf.b114

void main(void)
{
    init_gpio();
    LCD_Init();
    adc_init();
    pit_init();
    pwm_init();
    input_init();
    app_state_init();
    start_pic();

    while (1) {
        jianmian();
        if (g_app.started == 0) {
            continue;
        }
        switch (g_app.mode) {
#if FEATURE_GAME_SOKOBAN
        case APP_MODE_SOKOBAN:
            g_app.game_over = 0;
            tuixiangzi();
            break;
#endif
#if FEATURE_GAME_MINESWEEPER
        case APP_MODE_MINES:
            g_app.started = 1;
            g_app.playing = 1;
            saolei();
            break;
#endif
#if FEATURE_GAME_GOMOKU
        case APP_MODE_GOMOKU:
            g_app.playing = 0;
            g_app.gomoku_on = 1;
            wuziqi();
            g_app.gomoku_on = 0;
            break;
#endif
#if FEATURE_VIDEO_PLAYER
        case APP_MODE_FLASH:
            g_app.started = 1;
            g_app.playing = 0;
            flash_start();
            break;
#endif
        case APP_MODE_SYSTEM:
            g_app.started = 1;
            g_app.playing = 0;
            xtjianmian();
            break;
        default:
            break;
        }
        LCD_CLS();
        app_session_end();
        input_clear();
    }
}

void start_pic(void)
{
    uint8 i;

    g_app.game_over = 1;
    LCD_CLS();
    for (i = 0; i < 6; i++) {
        LCD_P14x16Ch(i * 16 + 10, 0, i + 66);
    }
    for (i = 0; i < 4; i++) {
        LCD_P14x16Ch(i * 15, 4, i + 66 + 6);
    }
    for (i = 0; i < 2; i++) {
        LCD_P14x16Ch(i * 22 + 15 * 4 + 14, 4, i + 66 + 10);
    }
    for (i = 0; i < 3; i++) {
        LCD_P14x16Ch(i * 18, 6, i + 66 + 12);
    }
    for (i = 0; i < 3; i++) {
        LCD_P14x16Ch(i * 15 + 15 * 4 + 14, 6, i + 66 + 15);
    }
    LCD_P8x16Str(58, 4, ":");
    LCD_P8x16Str(16 * 3, 6, ":");

    input_clear();
    while (1) {
        input_poll();
        if (input_edge(IN_UP) || input_edge(IN_DOWN) ||
            input_edge(IN_LEFT) || input_edge(IN_RIGHT) || input_edge(IN_OK)) {
            break;
        }
    }
    while (input_held(IN_UP) || input_held(IN_DOWN) ||
           input_held(IN_LEFT) || input_held(IN_RIGHT)) {
        input_poll();
    }
    LCD_CLS();
}

void die(FRESULT rc_err)
{
    LCD_P8x16Str(30, 2, "SD Error!");
    LCD_P8x16Str(30, 4, "Check it");
    (void)rc_err;
    rc = f_close(&fil);
    f_mount(0, NULL);
    while (1) {
    }
}

DWORD get_fattime(void)
{
    if (LPLD_RTC_IsRunning()) {
        uint32 unix_ts = LPLD_RTC_GetRealTime();
        uint32 days = unix_ts / 86400;
        uint32 rem = unix_ts % 86400;
        uint16 year = 1970, month = 1, day = 1;
        uint16 hour = rem / 3600;
        uint16 min = (rem % 3600) / 60;
        uint16 sec = rem % 60;
        static const uint16 mdays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        uint16 leap;
        while (days >= 365) {
            if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
                if (days >= 366) {
                    days -= 366;
                    year++;
                } else {
                    break;
                }
            } else {
                days -= 365;
                year++;
            }
        }
        leap = ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) ? 1 : 0;
        while (days >= (uint32)(mdays[month - 1] + (month == 2 ? leap : 0))) {
            days -= mdays[month - 1] + (month == 2 ? leap : 0);
            month++;
        }
        day = days + 1;
        return ((DWORD)(year - 1980) << 25)
             | ((DWORD)month << 21)
             | ((DWORD)day << 16)
             | ((DWORD)hour << 11)
             | ((DWORD)min << 5)
             | ((DWORD)sec >> 1);
    }
    return ((DWORD)(2018 - 1980) << 25)
         | ((DWORD)12 << 21)
         | ((DWORD)12 << 16)
         | ((DWORD)0 << 11)
         | ((DWORD)0 << 5)
         | ((DWORD)0 >> 1);
}

void readsd(void)
{
    DWORD fre_clust, fre_sect, tot_sect;
    unsigned char s[] = "00000";

    f_mount(0, &fatfs);
    rc = f_getfree("0:", &fre_clust, &fs);
    if (rc) {
        die(rc);
    }
    tot_sect = (fs->n_fatent - 2) * fs->csize;
    fre_sect = fre_clust * fs->csize;
    LCD_P14x16Ch(6, 2, 46);
    LCD_P14x16Ch(30 - 10, 2, 42);
    LCD_P14x16Ch(44 - 10, 2, 43);
    LCD_P8x16Str(59 - 10, 2, ":");
    LCD_P14x16Ch(30 - 10, 4, 44);
    LCD_P14x16Ch(44 - 10, 4, 45);
    LCD_P8x16Str(59 - 10, 4, ":");
    s[0] = (int)((fre_sect / 2 / 1024)) / 10000 + 48;
    s[1] = (int)((fre_sect / 2 / 1024)) / 1000 % 10 + 48;
    s[2] = (int)((fre_sect / 2 / 1024)) / 100 % 10 + 48;
    s[3] = (int)((fre_sect / 2 / 1024)) / 10 % 10 + 48;
    s[4] = (int)((fre_sect / 2 / 1024)) % 10 + 48;
    LCD_P8x16Str(58, 4, s);
    LCD_P8x16Str(58 + 5 * 8, 2, "MB");
    s[0] = (int)((tot_sect / 2 / 1024)) / 10000 + 48;
    s[1] = (int)((tot_sect / 2 / 1024)) / 1000 % 10 + 48;
    s[2] = (int)((tot_sect / 2 / 1024)) / 100 % 10 + 48;
    s[3] = (int)((tot_sect / 2 / 1024)) / 10 % 10 + 48;
    s[4] = (int)((tot_sect / 2 / 1024)) % 10 + 48;
    LCD_P8x16Str(58, 2, s);
    LCD_P8x16Str(58 + 5 * 8, 4, "MB");

    input_clear();
    while (g_app.started) {
        input_poll();
        if (input_edge(IN_BACK)) {
            g_app.started = 0;
        }
    }
    f_close(&fil);
    f_mount(0, NULL);
}

void badapple(uint8 spshu)
{
    rc = f_mount(0, &fatfs);
    if (rc) {
        die(rc);
    }
    switch (spshu) {
    case 1:
        rc = f_open(&fil, "0:/cartoon/badapple.bin", FA_READ);
        if (rc) {
            die(rc);
        }
        break;
    case 2:
        rc = f_open(&fil, "0:/cartoon/jljt.bin", FA_READ);
        if (rc) {
            die(rc);
        }
        break;
    }

    input_frame_ack();
    while (1) {
        switch (spshu) {
        case 1:
            rc = f_read(&fil, buff_86, sizeof(buff_86), &br);
            if (rc || !br) {
                goto done;
            }
            LCD_siping(21, 86);
            break;
        case 2:
            rc = f_read(&fil, buff_114, sizeof(buff_114), &br);
            if (rc || !br) {
                goto done;
            }
            LCD_siping(7, 114);
            break;
        }
        while (!input_frame_due()) {
            input_poll();
            if (g_app.playing == 0 || input_edge(IN_BACK)) {
                g_app.playing = 0;
                rc = f_close(&fil);
                f_mount(0, NULL);
                return;
            }
        }
        input_frame_ack();
        if (g_app.playing == 0) {
            rc = f_close(&fil);
            f_mount(0, NULL);
            return;
        }
    }
done:
    rc = f_close(&fil);
    f_mount(0, NULL);
    input_clear();
    while (g_app.playing) {
        input_poll();
        if (input_edge(IN_BACK)) {
            g_app.playing = 0;
        }
    }
}

void flash_start(void)
{
    uint8 spshu = 1;
    uint8 d = 1;

    LCD_P14x16Ch(105, spshu * 2 + 1, 16);
    input_clear();

    while (1) {
        while (d) {
            if (g_app.started == 0) {
                return;
            }
            input_poll();
            if (input_edge(IN_BACK)) {
                g_app.started = 0;
                return;
            }
            LCD_P8x16Str(16, 0, "Flash Player");
            LCD_P8x16Str(20, 3, "Bad Apple");
            LCD_P8x16Str(20, 5, "Garnidelia");
            LCD_P14x16Ch(105, spshu * 2 + 1, 16);
            if (input_edge(IN_UP)) {
                spshu--;
                playmusic(2);
                if (spshu == 0) {
                    spshu = 2;
                }
                LCD_P14x16Ch(105, 3, 15);
                LCD_P14x16Ch(105, 5, 15);
                LCD_P14x16Ch(105, spshu * 2 + 1, 16);
            }
            if (input_edge(IN_DOWN)) {
                spshu++;
                playmusic(2);
                if (spshu == 3) {
                    spshu = 1;
                }
                LCD_P14x16Ch(105, 3, 15);
                LCD_P14x16Ch(105, 5, 15);
                LCD_P14x16Ch(105, spshu * 2 + 1, 16);
            }
            if (input_edge(IN_LEFT) || input_edge(IN_RIGHT) || input_edge(IN_OK)) {
                d = 0;
            }
        }
        LCD_CLS();
        g_app.playing = 1;
        badapple(spshu);
        LCD_CLS();
        g_app.started = 1;
        d = 1;
        input_clear();
    }
}
