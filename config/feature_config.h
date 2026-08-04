/**
 * @file feature_config.h
 * @brief 功能裁剪开关 — 宏开关一键控制组件启停
 *
 * 目标9: 组件化工程解耦 — 支持宏开关一键功能裁剪
 * 目标8: 零开销可观测 — 调试/发布模式隔离
 */
#ifndef CONFIG_FEATURE_CONFIG_H
#define CONFIG_FEATURE_CONFIG_H

/*============================================================================
 * 编译模式选择
 *============================================================================*/
#ifndef FW_BUILD_MODE
  #define FW_BUILD_MODE  FW_RELEASE_MODE  /* 默认发布模式 */
#endif

#define FW_IS_DEBUG   (FW_BUILD_MODE == FW_DEBUG_MODE)
#define FW_IS_RELEASE (FW_BUILD_MODE == FW_RELEASE_MODE)

/*============================================================================
 * 功能组件开关 — 1=启用, 0=禁用
 *============================================================================*/

/* 游戏组件 */
#define FEATURE_GAME_SOKOBAN      1   /* 推箱子 */
#define FEATURE_GAME_MINESWEEPER  1   /* 扫雷 */
#define FEATURE_GAME_GOMOKU       1   /* 五子棋 */

/* 媒体组件 */
#define FEATURE_VIDEO_PLAYER      1   /* 视频播放器 */
#define FEATURE_AUDIO_WAV         1   /* WAV软解播放 */
#define FEATURE_AUDIO_MP3         1   /* VS1053 MP3硬解 */

/* 工具组件 */
#define FEATURE_MAP_EDITOR        1   /* 自定义地图绘制 */
#define FEATURE_SD_CARD_INFO      1   /* SD卡容量查看 */

/* 框架服务 */
#define FEATURE_MEMPOOL           1   /* 内存池 */
#define FEATURE_LOGGING           FW_IS_DEBUG  /* 日志仅调试模式 */
#define FEATURE_EVENT_SYSTEM      1   /* 事件系统 */

/* 调试选项 */
#define FEATURE_MEMPOOL_GUARD     FW_IS_DEBUG  /* 内存越界检测 */
#define FEATURE_ASSERT_CHECK      1   /* 断言检查 */
#define FEATURE_STACK_MONITOR     FW_IS_DEBUG  /* 栈监控 */

/*============================================================================
 * 日志等级 — 发布模式自动降级
 *============================================================================*/
#if FEATURE_LOGGING
  #define LOG_LEVEL  3  /* INFO */
#else
  #define LOG_LEVEL  0  /* NONE — 零开销 */
#endif

/*============================================================================
 * 事件队列大小
 *============================================================================*/
#define EVENT_QUEUE_SIZE  16

/*============================================================================
 * 编译期裁剪校验 — 确保依赖关系正确
 *============================================================================*/
#if FEATURE_AUDIO_MP3 && !defined(BOARD_VS1053_SPI)
  #error "MP3 feature requires VS1053 SPI configuration"
#endif

#if FEATURE_VIDEO_PLAYER && !defined(BOARD_VIDEO_PIT)
  #error "Video player requires PIT configuration"
#endif

#endif /* CONFIG_FEATURE_CONFIG_H */
