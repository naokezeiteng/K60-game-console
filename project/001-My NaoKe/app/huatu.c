#include "common.h"
#include "LQ12864.h"
#include "yingjian.h"
#include "huatu.h"

uint16 huatu1[8][8] = {0};
int16 dir1[4][2] = {-1, 0, 1, 0, 0, -1, 0, 1};
uint16 cu_x = 4;
uint16 cu_y = 4;
uint16 txshu = 1;

static void draw_map(void)
{
    uint16 i, j;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            LCD_P16x8Ch(i * 8, j, huatu1[i][j]);
        }
    }
}

void huatu(void)
{
    uint16 i, j;
    uint8 last_blink = 0;

    LCD_CLS();
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            huatu1[i][j] = 0;
        }
    }
    huatu1[2][2] = 2;
    for (i = 0; i < 2; ++i) {
        LCD_P14x16Ch1(72 + i * 14, 0, i + 46 + 1);
    }
    for (i = 0; i < 2; ++i) {
        LCD_P14x16Ch(77 + i * 14, 2, i + 48 + 1);
    }
    for (i = 0; i < 2; ++i) {
        LCD_P14x16Ch(82 + i * 14, 4, i + 50 + 1);
    }
    for (i = 0; i < 2; ++i) {
        LCD_P14x16Ch(87 + i * 14, 6, i + 52 + 1);
    }
    draw_map();
    input_clear();
    cu_x = 4;
    cu_y = 4;

    while (1) {
        if (g_app.started == 0) {
            LCD_CLS();
            return;
        }
        huatu1[2][2] = 2;
        input_poll();
        if (input_edge(IN_BACK)) {
            g_app.started = 0;
            LCD_CLS();
            return;
        }
        if (input_edge(IN_OK)) {
            g_app.tile_pick = 1;
            xuanzhetxjm();
        }
        if (input_edge(IN_UP)) {
            move(cu_x, cu_y, 2);
        } else if (input_edge(IN_DOWN)) {
            move(cu_x, cu_y, 3);
        } else if (input_edge(IN_LEFT)) {
            move(cu_x, cu_y, 0);
        } else if (input_edge(IN_RIGHT)) {
            move(cu_x, cu_y, 1);
        }

        {
            uint8 blink = (uint8)((input_ms() / 250u) & 1u);
            if (blink != last_blink) {
                last_blink = blink;
                if (blink) {
                    LCD_P16x8Ch(cu_x * 8, cu_y, 5);
                } else {
                    LCD_P16x8Ch(cu_x * 8, cu_y, huatu1[cu_x][cu_y]);
                }
            }
        }
    }
}

void move(uint16 x, uint16 y, uint16 id)
{
    int16 xx1, yy1;
    (void)x;
    (void)y;
    xx1 = (int16)cu_x + dir1[id][0];
    yy1 = (int16)cu_y + dir1[id][1];
    if (xx1 < 0 || xx1 >= 8 || yy1 < 0 || yy1 >= 8) {
        return;
    }
    LCD_P16x8Ch(cu_x * 8, cu_y, huatu1[cu_x][cu_y]);
    cu_x = (uint16)xx1;
    cu_y = (uint16)yy1;
}

void xuanzhetxjm(void)
{
    uint16 d = 1;
    input_clear();
    while (d) {
        input_poll();
        if (input_edge(IN_BACK)) {
            g_app.tile_pick = 0;
            return;
        }
        if (input_edge(IN_UP)) {
            txshu--;
            playmusic(2);
            showtx();
        }
        if (input_edge(IN_DOWN)) {
            txshu++;
            playmusic(2);
            if (txshu == 5) {
                txshu = 1;
            }
            showtx();
        }
        if (input_edge(IN_LEFT) || input_edge(IN_RIGHT) || input_edge(IN_OK)) {
            d = 0;
            g_app.tile_pick = 0;
            if (txshu == 1) {
                huatu1[cu_x][cu_y] = 0;
            }
            if (txshu == 2) {
                huatu1[cu_x][cu_y] = 1;
            }
            if (txshu == 3) {
                huatu1[cu_x][cu_y] = 3;
            }
            if (txshu == 4) {
                huatu1[cu_x][cu_y] = 4;
            }
            huatu1[2][2] = 2;
            LCD_P16x8Ch(cu_x * 8, cu_y, huatu1[cu_x][cu_y]);
        }
    }
}

void showtx(void)
{
    if (txshu == 0) {
        txshu = 4;
    }
    if (txshu == 1) {
        LCD_P14x16Ch1(72, 0, 47);
        LCD_P14x16Ch1(86, 0, 48);
        LCD_P14x16Ch(77, 2, 49);
        LCD_P14x16Ch(91, 2, 50);
        LCD_P14x16Ch(82, 4, 51);
        LCD_P14x16Ch(96, 4, 52);
        LCD_P14x16Ch(87, 6, 53);
        LCD_P14x16Ch(101, 6, 54);
    }
    if (txshu == 2) {
        LCD_P14x16Ch(72, 0, 47);
        LCD_P14x16Ch(86, 0, 48);
        LCD_P14x16Ch1(77, 2, 49);
        LCD_P14x16Ch1(91, 2, 50);
        LCD_P14x16Ch(82, 4, 51);
        LCD_P14x16Ch(96, 4, 52);
        LCD_P14x16Ch(87, 6, 53);
        LCD_P14x16Ch(101, 6, 54);
    }
    if (txshu == 3) {
        LCD_P14x16Ch(72, 0, 47);
        LCD_P14x16Ch(86, 0, 48);
        LCD_P14x16Ch(77, 2, 49);
        LCD_P14x16Ch(91, 2, 50);
        LCD_P14x16Ch1(82, 4, 51);
        LCD_P14x16Ch1(96, 4, 52);
        LCD_P14x16Ch(87, 6, 53);
        LCD_P14x16Ch(101, 6, 54);
    }
    if (txshu == 4) {
        LCD_P14x16Ch(72, 0, 47);
        LCD_P14x16Ch(86, 0, 48);
        LCD_P14x16Ch(77, 2, 49);
        LCD_P14x16Ch(91, 2, 50);
        LCD_P14x16Ch(82, 4, 51);
        LCD_P14x16Ch(96, 4, 52);
        LCD_P14x16Ch1(87, 6, 53);
        LCD_P14x16Ch1(101, 6, 54);
    }
}
