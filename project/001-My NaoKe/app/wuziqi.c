#include "common.h"
#include "LQ12864.h"
#include "wuziqi.h"
#include "yingjian.h"

extern uint8 board[8][8];
extern int8 codn_x, codn_y;
uint8 player = 1;
uint8 rou = 0;
static unsigned char round_str[] = "round:00";

void confirm_move(void)
{
    if (board[codn_y][codn_x / 8] != 0) {
        return;
    }
    board[codn_y][codn_x / 8] = player;
    if (player == 1) {
        rou++;
        round_str[6] = rou / 10 + 48;
        round_str[7] = rou % 10 + 48;
        LCD_P6x8Str(70, 5, round_str);
        player = 2;
        LCD_P8x16Str(70, 3, "PLAYER2");
    } else if (player == 2) {
        player = 1;
        LCD_P8x16Str(70, 3, "PLAYER1");
    }
    if (board_scan(3) == 1) {
        show_win();
    }
}

void wuziqi(void)
{
    int n, m;

    for (n = 0; n < 8; n++) {
        for (m = 0; m < 5; m++) {
            LCD_P14x16Ch(30 + m * 14, n, m + 33);
        }
        delay(80);
        LCD_Fill(0);
    }
    g_app.playing = 1;
    LCD_P8x16Str(70, 3, "PLAYER1");
    LCD_P8x16Str(74, 1, "GoBang");
    chess_board1();
    input_clear();
    {
        uint8 last_blink = 0xFF;
        while (g_app.started) {
            input_poll();
            if (input_edge(IN_BACK)) {
                if (g_app.playing == 0) {
                    LCD_CLS();
                    memset(board, 0, sizeof(board));
                    player = 1;
                    rou = 0;
                    g_app.playing = 1;
                    LCD_P8x16Str(70, 3, "PLAYER1");
                    LCD_P8x16Str(4, 1, "GoBang");
                    chess_board1();
                    last_blink = 0xFF;
                } else {
                    g_app.started = 0;
                }
            }
            if (g_app.playing) {
                key_scan();
                if (input_edge(IN_OK)) {
                    confirm_move();
                }
                if (g_app.playing && g_app.blink_on != last_blink) {
                    last_blink = g_app.blink_on;
                    if (g_app.blink_on) {
                        curCordBlink();
                    } else {
                        chess_board1();
                    }
                }
            }
        }
    }
    player = 1;
    rou = 0;
    codn_x = codn_y = 0;
    memset(board, 0, sizeof(board));
    g_app.playing = 0;
    g_app.started = 0;
}

void show_win(void)
{
    g_app.playing = 0;
    LCD_CLS();
    chess_board();
    if (player == 2) {
        LCD_P6x8Str(84, 7, "PLAYER1");
    } else if (player == 1) {
        LCD_P6x8Str(84, 7, "PLAYER2");
    }
}

void key_scan(void)
{
    if (input_edge(IN_RIGHT)) {
        codn_x += 8;
        if (codn_x > 56) {
            codn_x = 0;
        }
    } else if (input_edge(IN_LEFT)) {
        codn_x -= 8;
        if (codn_x < 0) {
            codn_x = 56;
        }
    }
    if (input_edge(IN_DOWN)) {
        codn_y++;
        if (codn_y > 7) {
            codn_y = 0;
        }
    } else if (input_edge(IN_UP)) {
        codn_y--;
        if (codn_y < 0) {
            codn_y = 7;
        }
    }
}

uint8 board_scan(int num)
{
    uint8 count = 0;
    int i, j, a;

    for (i = 0; i < 8; i++) {
        count = 0;
        for (j = 0; j < 7; j++) {
            uint8 tmp = board[i][j];
            if (tmp != board[i][j + 1] || tmp == 0) {
                count = 0;
            } else {
                count++;
            }
            if (count > num) {
                return 1;
            }
        }
    }
    for (i = 0; i < 8; i++) {
        count = 0;
        for (j = 0; j < 7; j++) {
            uint8 tmp = board[j][i];
            if (tmp != board[j + 1][i] || tmp == 0) {
                count = 0;
            } else {
                count++;
            }
            if (count > num) {
                return 1;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        count = 0;
        a = i;
        for (j = 0; j < 8 - i; j++) {
            uint8 tmp = board[a][j];
            if (tmp != board[a + 1][j + 1] || tmp == 0) {
                count = 0;
            } else {
                count++;
            }
            if (count > 3) {
                return 1;
            }
            a++;
        }
    }
    for (i = 0; i < 4; i++) {
        count = 0;
        a = 0;
        for (j = 4 - i; j < 8; j++) {
            uint8 tmp = board[a][j];
            if (tmp != board[a + 1][j + 1] || tmp == 0) {
                count = 0;
            } else {
                count++;
            }
            if (count > 3) {
                return 1;
            }
            a++;
        }
    }
    for (i = 0; i < 4; i++) {
        count = 0;
        a = i;
        for (j = 7; j > i; j--) {
            uint8 tmp = board[a][j];
            if (tmp != board[a + 1][j - 1] || tmp == 0) {
                count = 0;
            } else {
                count++;
            }
            if (count > 3) {
                return 1;
            }
            a++;
        }
    }
    for (i = 0; i < 3; i++) {
        count = 0;
        a = 0;
        for (j = 4 + i; j > 0; j--) {
            uint8 tmp = board[a][j];
            if (tmp != board[a + 1][j - 1] || tmp == 0) {
                count = 0;
            } else {
                count++;
            }
            if (count > 3) {
                return 1;
            }
            a++;
        }
    }
    return 0;
}
