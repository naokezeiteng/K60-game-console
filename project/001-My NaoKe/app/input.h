/**
 * @file input.h
 * @brief 输入与节拍 — 摇杆边沿/连发、按键 ISR 置位、PIT 毫秒时间
 *
 * ISR 只置标志或累加节拍，禁止 delay / LCD / 蜂鸣。
 * 主循环每圈调用 input_poll()，再用 input_edge() 消费事件。
 */
#ifndef INPUT_H
#define INPUT_H

#include "common.h"

enum {
    IN_UP = 0,
    IN_DOWN,
    IN_LEFT,
    IN_RIGHT,
    IN_OK,
    IN_BACK,
    IN_COUNT
};

void input_init(void);
void input_poll(void);
void input_clear(void);

uint32 input_ms(void);

/* 本圈边沿（含连发），读一次即清除 */
uint8 input_edge(uint8 id);
/* 当前是否按住 */
uint8 input_held(uint8 id);

void input_beep(uint16 kind);

/* 视频帧节拍（PIT0） */
void input_frame_ack(void);
uint8 input_frame_due(void);

/* 仅供 yingjian 中断调用 */
void input_on_ok_isr(void);
void input_on_back_isr(void);
void input_tick_isr(void);
void input_video_isr(void);

#endif /* INPUT_H */
