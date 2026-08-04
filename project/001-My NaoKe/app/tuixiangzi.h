#ifndef  _TUIXIANGZI_H  //必须确保header guard宏名永不重名
#define  _TUIXIANGZI_H
void start(void);
void show_map(void);
void update(uint16 x, uint16 y);
void show_start(void);
uint16 judge(uint16 x,uint16 y,uint16 id);
void fun(uint16 key,uint16 dirction);
void tuixiangzi(void);
#endif
