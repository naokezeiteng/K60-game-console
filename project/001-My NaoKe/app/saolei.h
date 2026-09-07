#ifndef _SAOLEI_H
#define _SAOLEI_H
#define rows 8
#define cols 8
#define Count 10
void saolei(void);
uint16 menu(void);
void display(unsigned char show[rows][cols]);
uint16 Game(unsigned char mine[rows][cols], unsigned char show[rows][cols]);
void set_mine(unsigned char mine[rows][cols]);
uint16 Sweep(unsigned char mine[rows][cols], unsigned char show[rows][cols]);
uint16 get_num(unsigned char mine[rows][cols], uint16 x, uint16 y);
void kongzhi(void);
void time1(void);
void delay1(uint16 ms);
void surrond(uint16 a, uint16 b);
void panduan(uint16 a, uint16 b);
#endif
