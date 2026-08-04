/**
 * @file log.h
 * @brief 零开销分级日志系统
 *
 * 目标6: 零开销分级日志
 *   高于当前 LOG_LEVEL 的日志宏在预处理期被完全消除, 不产生任何代码、不评估参数。
 *   启用的日志经 log_printf() 统一路由到可配置输出 (UART/LCD/RTT)。
 *
 * 目标2: 自定义段自动注册架构
 *   log_init() 通过 FW_AUTO_REGISTER 注册, 系统启动自动调用。
 *
 * 目标7: 编译期强类型风控
 *   日志等级为强类型枚举, 编译期等级比较决定是否编译日志调用。
 *
 * IAR C90 注意:
 *   - 宏内不使用行注释, 一律采用块注释或行续反斜杠。
 *   - 变参宏使用 ##__VA_ARGS__ (IAR 扩展支持), 兼容零额外参数调用。
 */
#ifndef FRAMEWORK_LOG_LOG_H
#define FRAMEWORK_LOG_LOG_H

#include "compiler.h"
#include <stdint.h>

/*============================================================================
 * 日志等级 — 数值越大越详细
 *
 * 注意: 枚举常量在 #if 中会被当作 0, 故预处理器阈值比较必须用下面的
 *       整数宏 FW_LOG_LV_*; 枚举仅用于运行期强类型 (fw_log_level_t)。
 *============================================================================*/
#define FW_LOG_LV_NONE   0   /* 关闭所有日志 */
#define FW_LOG_LV_ERROR  1   /* 仅错误 */
#define FW_LOG_LV_WARN   2   /* 错误 + 警告 */
#define FW_LOG_LV_INFO   3   /* + 一般信息 (默认) */
#define FW_LOG_LV_DEBUG  4   /* + 调试信息 */
#define FW_LOG_LV_TRACE  5   /* + 详细跟踪 */

typedef enum {
    LOG_LEVEL_NONE  = FW_LOG_LV_NONE,
    LOG_LEVEL_ERROR = FW_LOG_LV_ERROR,
    LOG_LEVEL_WARN  = FW_LOG_LV_WARN,
    LOG_LEVEL_INFO  = FW_LOG_LV_INFO,
    LOG_LEVEL_DEBUG = FW_LOG_LV_DEBUG,
    LOG_LEVEL_TRACE = FW_LOG_LV_TRACE
} fw_log_level_t;

/**
 * 编译期日志阈值 (整数), 严格高于此值的日志宏在预处理期被消除 (零开销)。
 * 默认 LOG_LEVEL_INFO; 调整时传整数或 FW_LOG_LV_* 宏, 例如:
 *   -DLOG_LEVEL=FW_LOG_LV_DEBUG   或   -DLOG_LEVEL=4
 * 请勿传枚举名 (枚举在 #if 中恒为 0)。
 */
#ifndef LOG_LEVEL
  #define LOG_LEVEL  FW_LOG_LV_INFO
#endif

/*============================================================================
 * 输出函数类型与配置
 *============================================================================*/
typedef void (*fw_log_output_fn)(const char *msg);

/** 安装输出目的地 (UART/LCD/RTT...), 传 NULL 恢复默认空实现 */
void log_set_output(fw_log_output_fn fn);

/** 底层格式化入口 — 由各 LOG_xxx 宏调用 */
void log_printf(fw_log_level_t level, const char *file, int line,
                const char *fmt, ...);

/*============================================================================
 * 模块标签 — 每个模块用 LOG_TAG_DEFINE 声明一个标识串
 *
 * 用法:
 *   LOG_TAG_DEFINE(MAIN);            生成 static const char _log_tag[] = "MAIN";
 *   #define FW_LOG_TAG  _log_tag     (可选) 让本文件日志以 "MAIN" 作为模块名
 *
 * 不定义 FW_LOG_TAG 时, 日志宏使用 __FILE__ 作为模块标识。
 *============================================================================*/
#define LOG_TAG_DEFINE(tag)  static const char _log_tag[] = #tag

#ifndef FW_LOG_TAG
  #define FW_LOG_TAG  __FILE__
#endif

/*============================================================================
 * 日志宏 — 等级被关闭时展开为 ((void)0), 零代码、零参数评估
 * 启用时调用 log_printf(level, FW_LOG_TAG, __LINE__, fmt, ##__VA_ARGS__)
 *============================================================================*/
#if (LOG_LEVEL >= FW_LOG_LV_ERROR)
  #define LOG_ERROR(fmt, ...)  log_printf(LOG_LEVEL_ERROR, FW_LOG_TAG, __LINE__, fmt, ##__VA_ARGS__)
#else
  #define LOG_ERROR(fmt, ...)  ((void)0)
#endif

#if (LOG_LEVEL >= FW_LOG_LV_WARN)
  #define LOG_WARN(fmt, ...)   log_printf(LOG_LEVEL_WARN,  FW_LOG_TAG, __LINE__, fmt, ##__VA_ARGS__)
#else
  #define LOG_WARN(fmt, ...)   ((void)0)
#endif

#if (LOG_LEVEL >= FW_LOG_LV_INFO)
  #define LOG_INFO(fmt, ...)   log_printf(LOG_LEVEL_INFO,  FW_LOG_TAG, __LINE__, fmt, ##__VA_ARGS__)
#else
  #define LOG_INFO(fmt, ...)   ((void)0)
#endif

#if (LOG_LEVEL >= FW_LOG_LV_DEBUG)
  #define LOG_DEBUG(fmt, ...)  log_printf(LOG_LEVEL_DEBUG, FW_LOG_TAG, __LINE__, fmt, ##__VA_ARGS__)
#else
  #define LOG_DEBUG(fmt, ...)  ((void)0)
#endif

#if (LOG_LEVEL >= FW_LOG_LV_TRACE)
  #define LOG_TRACE(fmt, ...)  log_printf(LOG_LEVEL_TRACE, FW_LOG_TAG, __LINE__, fmt, ##__VA_ARGS__)
#else
  #define LOG_TRACE(fmt, ...)  ((void)0)
#endif

/*============================================================================
 * 十六进制转储 — 仅在 LOG_LEVEL >= DEBUG 时编译
 *============================================================================*/
#if (LOG_LEVEL >= FW_LOG_LV_DEBUG)
  void log_hex_dump(const void *data, uint32_t len);
  #define LOG_HEX_DUMP(data, len)  log_hex_dump((data), (len))
#else
  #define LOG_HEX_DUMP(data, len)  ((void)0)
#endif

/*============================================================================
 * 初始化 (由 FW_AUTO_REGISTER 自动注册, 一般无需手动调用)
 *============================================================================*/
void log_init(void);

#endif /* FRAMEWORK_LOG_LOG_H */
