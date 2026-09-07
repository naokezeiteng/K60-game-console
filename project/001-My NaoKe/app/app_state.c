#include "app_state.h"

app_state_t g_app;

void app_state_init(void)
{
    g_app.mode      = APP_MODE_SOKOBAN;
    g_app.started   = 0;
    g_app.playing   = 0;
    g_app.in_menu   = 0;
    g_app.game_over = 0;
    g_app.exit_req  = 0;
    g_app.need_ack  = 0;
    g_app.gomoku_on = 0;
    g_app.tile_pick = 0;
    g_app.timer_on  = 0;
    g_app.time_left = 0;
    g_app.blink_on  = 0;
    g_app.beep_warn = 0;
}

void app_session_end(void)
{
    g_app.started   = 0;
    g_app.playing   = 0;
    g_app.in_menu   = 0;
    g_app.game_over = 0;
    g_app.exit_req  = 0;
    g_app.need_ack  = 0;
    g_app.gomoku_on = 0;
    g_app.tile_pick = 0;
    g_app.timer_on  = 0;
    g_app.blink_on  = 0;
    g_app.beep_warn = 0;
}
