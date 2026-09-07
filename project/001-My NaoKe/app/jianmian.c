#include "common.h"
#include "LQ12864.h"
#include "jianmian.h"
#include "yingjian.h"
#include "huatu.h"

static void menu_draw(uint8 sel)
{
    uint8 i;
    uint8 jma[5] = {0, 0, 0, 0, 0};

    jma[sel - 1] = 1;
    for (i = 0; i < 5; i++) {
        LCD_tuxing(i * 26, 0, jma[i]);
    }
    for (i = 0; i < 6; i++) {
        LCD_tuxing1(0, i + 2, 0);
        LCD_tuxing1(127, i + 2, 0);
    }
    for (i = 2; i < 126; i++) {
        LCD_tuxing1(i, 7, 1);
    }

    switch (sel) {
    case APP_MODE_SOKOBAN:
        for (i = 0; i < 3; ++i) {
            LCD_P14x16Ch(72 + 14 * i, 3, i + 6);
        }
        LCD_tuxing2(15, 3, 0);
        break;
    case APP_MODE_MINES:
        for (i = 0; i < 2; ++i) {
            LCD_P14x16Ch(72 + 14 * i, 3, i + 28);
        }
        LCD_P14x16Ch(72 + 14 * 2, 3, 15);
        LCD_tuxing2(15, 3, 2);
        break;
    case APP_MODE_GOMOKU:
        for (i = 0; i < 3; ++i) {
            LCD_P14x16Ch(72 + 14 * i, 3, i + 34);
        }
        LCD_tuxing2(15, 3, 1);
        break;
    case APP_MODE_FLASH:
        LCD_P8x16Str(72, 3, "Flash ");
        LCD_tuxing2(15, 3, 3);
        break;
    case APP_MODE_SYSTEM:
        for (i = 0; i < 2; ++i) {
            LCD_P14x16Ch(72 + 14 * i, 3, i + 57);
        }
        LCD_P14x16Ch(72 + 14 * 2, 3, 15);
        LCD_tuxing2(15, 3, 4);
        break;
    }
}

void jianmian(void)
{
    g_app.in_menu = 1;
    input_clear();
    menu_draw(g_app.mode);

    while (1) {
        input_poll();
        if (input_edge(IN_LEFT)) {
            if (g_app.mode == APP_MODE_SOKOBAN) {
                g_app.mode = APP_MODE_SYSTEM;
            } else {
                g_app.mode--;
            }
            playmusic(2);
            menu_draw(g_app.mode);
        }
        if (input_edge(IN_RIGHT)) {
            if (g_app.mode == APP_MODE_SYSTEM) {
                g_app.mode = APP_MODE_SOKOBAN;
            } else {
                g_app.mode++;
            }
            playmusic(2);
            menu_draw(g_app.mode);
        }
        if (input_edge(IN_OK)) {
            break;
        }
    }

    g_app.in_menu = 0;
    g_app.started = 1;
    LCD_CLS();
}

void xtjianmian(void)
{
    uint8 xtshu = 1;
    uint8 d = 1;

    LCD_P14x16Ch(105, xtshu * 2 + 1, 16);
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

            LCD_P14x16Ch(40, 0, 64);
            LCD_P14x16Ch(70, 0, 65);
            LCD_P14x16Ch(25, 3, 59);
            LCD_P14x16Ch(41, 3, 60);
            LCD_P14x16Ch(57, 3, 61);
            LCD_P14x16Ch(73, 3, 62);
            LCD_P14x16Ch(89, 3, 63);
            LCD_P8x16Str(25, 5, "  SD Card");
            LCD_P14x16Ch(105, xtshu * 2 + 1, 16);

            if (input_edge(IN_UP)) {
                xtshu--;
                playmusic(2);
                if (xtshu == 0) {
                    xtshu = 2;
                }
                LCD_P14x16Ch(105, 3, 15);
                LCD_P14x16Ch(105, 5, 15);
                LCD_P14x16Ch(105, xtshu * 2 + 1, 16);
            }
            if (input_edge(IN_DOWN)) {
                xtshu++;
                playmusic(2);
                if (xtshu == 3) {
                    xtshu = 1;
                }
                LCD_P14x16Ch(105, 3, 15);
                LCD_P14x16Ch(105, 5, 15);
                LCD_P14x16Ch(105, xtshu * 2 + 1, 16);
            }
            if (input_edge(IN_LEFT) || input_edge(IN_RIGHT) || input_edge(IN_OK)) {
                d = 0;
            }
        }
        LCD_CLS();
        g_app.playing = 1;
        if (xtshu == 1) {
#if FEATURE_MAP_EDITOR
            g_app.mode = APP_MODE_MAPEDIT;
            huatu();
            g_app.mode = APP_MODE_SYSTEM;
#endif
        } else {
#if FEATURE_SD_CARD_INFO
            readsd();
#endif
        }
        LCD_CLS();
        g_app.started = 1;
        d = 1;
        input_clear();
    }
}
