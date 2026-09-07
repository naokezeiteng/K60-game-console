/**
 * @file app_state.h
 * @brief 会话状态 — 菜单/游戏共享标志，替代散落的 extern 全局变量
 *
 * ISR 只允许写本结构中标记为“中断置位”的字段，游戏逻辑在主循环消费。
 */
#ifndef APP_STATE_H
#define APP_STATE_H

#include "common.h"

enum {
    APP_MODE_SOKOBAN = 1,
    APP_MODE_MINES   = 2,
    APP_MODE_GOMOKU  = 3,
    APP_MODE_FLASH   = 4,
    APP_MODE_SYSTEM  = 5,
    APP_MODE_MAPEDIT = 6
};

typedef struct {
    volatile uint8  mode;       /* 当前菜单项 1..5，画图时临时为 6 */
    volatile uint8  started;    /* 已进入某功能（原 gamestart） */
    volatile uint8  playing;    /* 功能内部“进行中”（原 gameing） */
    volatile uint8  in_menu;    /* 主菜单等待确认 */
    volatile uint8  game_over;  /* 推箱子胜负画面 */
    volatile uint8  exit_req;   /* 请求退回上一级 */
    volatile uint8  need_ack;   /* 胜负画面等待按键 */
    volatile uint8  gomoku_on;  /* 五子棋对局中，供 200ms 闪烁 */
    volatile uint8  tile_pick;  /* 画图：选择图块 */
    volatile uint8  timer_on;   /* 推箱子倒计时使能 */
    volatile uint16 time_left;  /* 推箱子剩余秒 */
    volatile uint8  blink_on;   /* 五子棋光标闪烁相位 */
    volatile uint8  beep_warn;  /* 倒计时警告蜂鸣，主循环消费 */
} app_state_t;

extern app_state_t g_app;

void app_state_init(void);
void app_session_end(void);

#endif /* APP_STATE_H */
