#include "input.h"
#include "app_state.h"
#include "board_config.h"

#define INPUT_REPEAT_FIRST_MS  220u
#define INPUT_REPEAT_MS        140u
#define INPUT_BTN_COOL_MS      40u

static volatile uint32 s_ms;
static volatile uint8  s_ok_pend;
static volatile uint8  s_back_pend;
static volatile uint8  s_frame_due;
static volatile uint8  s_div200;

static uint8  s_held[IN_COUNT];
static uint8  s_edge[IN_COUNT];
static uint32 s_rep_at[IN_COUNT];
static uint32 s_beep_until;
static uint32 s_last_ok;
static uint32 s_last_back;

static void input_game_tick_200ms(void);

void input_init(void)
{
    s_ms = 0;
    s_ok_pend = 0;
    s_back_pend = 0;
    s_frame_due = 0;
    s_div200 = 0;
    s_beep_until = 0;
    s_last_ok = 0;
    s_last_back = 0;
    input_clear();
}

void input_clear(void)
{
    uint8 i;
    for (i = 0; i < IN_COUNT; i++) {
        s_held[i] = 0;
        s_edge[i] = 0;
        s_rep_at[i] = 0;
    }
    s_ok_pend = 0;
    s_back_pend = 0;
}

uint32 input_ms(void)
{
    return s_ms;
}

uint8 input_edge(uint8 id)
{
    uint8 e;
    if (id >= IN_COUNT) {
        return 0;
    }
    e = s_edge[id];
    s_edge[id] = 0;
    return e;
}

uint8 input_held(uint8 id)
{
    if (id >= IN_COUNT) {
        return 0;
    }
    return s_held[id];
}

void input_on_ok_isr(void)
{
    s_ok_pend = 1;
}

void input_on_back_isr(void)
{
    s_back_pend = 1;
}

void input_video_isr(void)
{
    s_frame_due = 1;
}

uint8 input_frame_due(void)
{
    return s_frame_due;
}

void input_frame_ack(void)
{
    s_frame_due = 0;
}

void input_tick_isr(void)
{
    s_ms += 10u;
    s_div200++;
    if (s_div200 >= 20u) {
        s_div200 = 0;
        input_game_tick_200ms();
    }
}

/* 200ms 节拍：只改计数/标志，不碰 LCD */
static void input_game_tick_200ms(void)
{
    static uint8 sec_div;

    if (g_app.gomoku_on && g_app.playing) {
        g_app.blink_on ^= 1;
    }

    sec_div++;
    if (sec_div < 5u) {
        return;
    }
    sec_div = 0;

    if (g_app.mode == APP_MODE_SOKOBAN && g_app.timer_on) {
        if (g_app.time_left > 0) {
            g_app.time_left--;
        }
        if (g_app.time_left <= 10u && g_app.game_over == 0) {
            g_app.beep_warn = 1;
        }
    }
}

void input_beep(uint16 kind)
{
    if (kind == 2) {
        LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch0, 30);
        s_beep_until = s_ms + 40u;
    } else {
        LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch0, 10000);
        s_beep_until = s_ms + 80u;
    }
}

static void joy_track(uint8 id, uint8 now_held)
{
    uint32 now = s_ms;

    if (now_held) {
        if (s_held[id] == 0) {
            s_edge[id] = 1;
            s_rep_at[id] = now + INPUT_REPEAT_FIRST_MS;
        } else if ((int32)(now - s_rep_at[id]) >= 0) {
            s_edge[id] = 1;
            s_rep_at[id] = now + INPUT_REPEAT_MS;
        }
        s_held[id] = 1;
    } else {
        s_held[id] = 0;
        s_rep_at[id] = 0;
    }
}

void input_poll(void)
{
    uint16 ax, ay;
    uint32 now;
    uint8 up, down, left, right;

    now = s_ms;

    if (s_beep_until != 0 && (int32)(now - s_beep_until) >= 0) {
        LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch0, 0);
        s_beep_until = 0;
    }

    if (g_app.beep_warn) {
        g_app.beep_warn = 0;
        input_beep(1);
    }

    ax = LPLD_ADC_Get(BOARD_JOY_ADC, BOARD_JOY_X_CH);
    ay = LPLD_ADC_Get(BOARD_JOY_ADC, BOARD_JOY_Y_CH);

    left  = (ax < BOARD_JOY_THRESHOLD_LOW)  ? 1 : 0;
    right = (ax > BOARD_JOY_THRESHOLD_HIGH) ? 1 : 0;
    up    = (ay < BOARD_JOY_THRESHOLD_LOW)  ? 1 : 0;
    down  = (ay > BOARD_JOY_THRESHOLD_HIGH) ? 1 : 0;

    joy_track(IN_LEFT,  left);
    joy_track(IN_RIGHT, right);
    joy_track(IN_UP,    up);
    joy_track(IN_DOWN,  down);

    if (s_ok_pend) {
        s_ok_pend = 0;
        if ((now - s_last_ok) >= INPUT_BTN_COOL_MS) {
            s_edge[IN_OK] = 1;
            s_held[IN_OK] = 1;
            s_last_ok = now;
        }
    } else {
        s_held[IN_OK] = 0;
    }

    if (s_back_pend) {
        s_back_pend = 0;
        if ((now - s_last_back) >= INPUT_BTN_COOL_MS) {
            s_edge[IN_BACK] = 1;
            s_held[IN_BACK] = 1;
            s_last_back = now;
        }
    } else {
        s_held[IN_BACK] = 0;
    }
}
