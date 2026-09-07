#include "common.h"
#include "LQ12864.h"
#include "saolei.h"
#include "yingjian.h"

uint16 gameoversl = 0;
uint16 t1 = 0;
uint16 t2 = 0;
uint16 t3 = 0;
uint16 t4 = 0;
uint16 t5 = 0;
uint16 t6 = 0;
uint16 ct = 0;
unsigned char mine[rows][cols];
unsigned char show[rows][cols];
uint16 x = 0;
uint16 y = 0;
uint16 biaozhi = 0;
uint16 count = Count;
uint16 cit = 0;
uint16 k = 0;
static uint32 s_start_ms;
unsigned char s[] = "0";
unsigned char f[] = "0";
unsigned char g[] = "0";

static void saolei_reset(void)
{
    gameoversl = 0;
    t1 = t2 = t3 = t4 = t5 = t6 = ct = x = y = biaozhi = cit = k = 0;
    count = Count;
}

void saolei(void)
{
    uint16 i, j;

    g_app.started = 1;
    gameoversl = 1;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            mine[i][j] = '0';
            show[i][j] = '*';
        }
    }
    menu();
    s_start_ms = input_ms();
    input_clear();
    while (1) {
        g_app.started = 1;
        input_poll();
        if (input_edge(IN_BACK)) {
            g_app.started = 0;
            g_app.playing = 0;
            saolei_reset();
            return;
        }
        if (t1 == 0 && t2 == 0) {
            LCD_CLS();
            t2 = 1;
        }
        if (cit == 0 && input_edge(IN_OK)) {
            if (mine[x][y] == '1') {
                ct = 1;
            } else {
                biaozhi = 0;
                panduan(x, y);
            }
        }
        if (cit == 0) {
            LCD_P8x8Ch(8 * x, y, 0);
        }
        Game(mine, show);
        if (gameoversl == 1 && (cit == 2 || cit == 1)) {
            /* 胜负画面：再按返回退出 */
            if (input_edge(IN_OK) || g_app.started == 0) {
                /* 停留直到 BACK，由上面 IN_BACK 处理 */
            }
        }
    }
}

uint16 menu(void)
{
    return 0;
}

void set_mine(unsigned char mine_map[rows][cols])
{
    uint16 n = 0;
    uint16 m = 0;
    static uint32 seed = 0;
    if (seed == 0) {
        seed = (LPLD_ADC_Get(ADC1, DAD1) << 16) | LPLD_ADC_Get(ADC1, DAD1);
        seed ^= input_ms();
    }
    if (count == 0) {
        return;
    }
    while (count) {
        seed = seed * 1103515245 + 12345;
        n = (seed >> 16) % 8;
        seed = seed * 1103515245 + 12345;
        m = (seed >> 16) % 8;
        if (mine_map[n][m] == '0') {
            mine_map[n][m] = '1';
            count--;
        }
    }
}

void display(unsigned char show_map[rows][cols])
{
    uint16 i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            s[0] = show_map[i][j];
            LCD_P6x8Str(8 * i, j, s);
        }
    }
}

uint16 get_num(unsigned char mine_map[rows][cols], uint16 px, uint16 py)
{
    int16 n = 0;
    int16 i, j;
    for (i = -1; i <= 1; i++) {
        for (j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            if ((int16)px + i >= 0 && (int16)px + i <= 7 &&
                (int16)py + j >= 0 && (int16)py + j <= 7) {
                if (mine_map[px + i][py + j] == '1') {
                    n++;
                }
            }
        }
    }
    return (uint16)n;
}

uint16 Sweep(unsigned char mine_map[rows][cols], unsigned char show_map[rows][cols])
{
    (void)mine_map;
    if (cit == 0) {
        if (input_edge(IN_UP) && y > 0) {
            y--;
        }
        if (input_edge(IN_DOWN) && y < 7) {
            y++;
        }
        if (input_edge(IN_LEFT) && x > 0) {
            x--;
        }
        if (input_edge(IN_RIGHT) && x < 7) {
            x++;
        }
    }

    if (cit == 0) {
        display(show_map);
        k = 0;
        {
            uint16 i, j;
            for (i = 0; i < rows; i++) {
                for (j = 0; j < cols; j++) {
                    if (show_map[i][j] == '*') {
                        k++;
                    }
                }
            }
        }
        sprintf(g, "%d", k);
        LCD_P8x16Str(70, 3, "*");
        LCD_P14x16Ch(78, 3, 26);
        LCD_P14x16Ch(92, 3, 27);
        if (t6 == 0 && g[0] < 10) {
            LCD_CLS();
            t6 = 1;
        }
        LCD_P8x16Str(106, 3, g);
        if (k <= 15 && k > Count) {
            LCD_P14x16Ch(70, 0, 23);
            LCD_P14x16Ch(84, 0, 24);
            LCD_P14x16Ch(98, 0, 25);
        }
        if (k == Count) {
            cit = 2;
            if (t5 == 0) {
                LCD_CLS();
                t5 = 1;
            }
            LCD_P8x16Str(30, 2, "you are a ");
            LCD_P8x16Str(40, 4, " winner ");
            gameoversl = 1;
        }
        if (ct == 1) {
            if (t4 == 0) {
                LCD_CLS();
                t4 = 1;
            }
            cit = 2;
            LCD_P8x16Str(20, 3, "try again ! ");
            gameoversl = 1;
        }
    }
    return 0;
}

uint16 Game(unsigned char mine_map[rows][cols], unsigned char show_map[rows][cols])
{
    time1();
    set_mine(mine_map);
    if (cit == 0) {
        display(show_map);
    }
    Sweep(mine_map, show_map);
    return 0;
}

void kongzhi(void)
{
}

void delay1(uint16 ms)
{
    delay((int)ms);
}

void time1(void)
{
    uint16 timesl = (uint16)((input_ms() - s_start_ms) / 1000u);
    if (cit == 0) {
        sprintf(f, "%d", timesl);
        LCD_P8x16Str(100, 6, f);
        LCD_P14x16Ch(70, 6, 17);
        LCD_P14x16Ch(84, 6, 18);
    }
    if (timesl >= 200 && cit != 2) {
        cit = 1;
        if (t3 == 0) {
            LCD_CLS();
            t3 = 1;
        }
        LCD_P8x16Str(10, 2, "you are out of ");
        LCD_P8x16Str(40, 4, " time ");
        gameoversl = 1;
    }
}

void surrond(uint16 a, uint16 b)
{
    if ((a + 1) <= 7 && (b + 1) <= 7) {
        if (mine[a + 1][b + 1] != '1') {
            show[a + 1][b + 1] = get_num(mine, a + 1, b + 1) + '0';
        }
    }
    if ((b + 1) <= 7) {
        if (mine[a][b + 1] != '1') {
            show[a][b + 1] = get_num(mine, a, b + 1) + '0';
        }
    }
    if ((a + 1) <= 7) {
        if (mine[a + 1][b] != '1') {
            show[a + 1][b] = get_num(mine, a + 1, b) + '0';
        }
    }
    if (a >= 1 && (b + 1) <= 7) {
        if (mine[a - 1][b + 1] != '1') {
            show[a - 1][b + 1] = get_num(mine, a - 1, b + 1) + '0';
        }
    }
    if (a >= 1 && b >= 1) {
        if (mine[a - 1][b - 1] != '1') {
            show[a - 1][b - 1] = get_num(mine, a - 1, b - 1) + '0';
        }
    }
    if ((a + 1) <= 7 && b >= 1) {
        if (mine[a + 1][b - 1] != '1') {
            show[a + 1][b - 1] = get_num(mine, a + 1, b - 1) + '0';
        }
    }
    if (b >= 1) {
        if (mine[a][b - 1] != '1') {
            show[a][b - 1] = get_num(mine, a, b - 1) + '0';
        }
    }
    if (a >= 1) {
        if (mine[a - 1][b] != '1') {
            show[a - 1][b] = get_num(mine, a - 1, b) + '0';
        }
    }
}

void panduan(uint16 a, uint16 b)
{
    int16 i, j;
    if (mine[a][b] != '1') {
        uint16 ret = get_num(mine, a, b);
        if (ret != 0) {
            show[a][b] = ret + '0';
            return;
        }
        if (biaozhi <= 80 && ret == 0) {
            show[a][b] = ret + '0';
            surrond(a, b);
            biaozhi++;
            for (i = -1; i <= 1; i++) {
                for (j = -1; j <= 1; j++) {
                    int16 ni = (int16)a + i;
                    int16 nj = (int16)b + j;
                    if (i == 0 && j == 0) {
                        continue;
                    }
                    if (ni >= 0 && ni <= 7 && nj >= 0 && nj <= 7) {
                        if (show[ni][nj] != '*') {
                            continue;
                        }
                        if (get_num(mine, (uint16)ni, (uint16)nj) == 0) {
                            panduan((uint16)ni, (uint16)nj);
                        }
                    }
                }
            }
        }
    }
}
