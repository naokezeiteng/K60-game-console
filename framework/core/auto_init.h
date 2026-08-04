/**
 * @file auto_init.h
 * @brief 自定义段自动注册架构 — 驱动/模块/钩子全自动注册
 *
 * 目标2: 自定义段自动注册架构
 * 依托链接脚本自定义功能段，模块通过宏声明即可自动注册到全局表，
 * 无需手动维护初始化列表，实现零框架改动的可插拔扩展。
 *
 * 原理:
 *   每个模块用 FW_AUTO_REGISTER() 宏声明一个 init_entry_t，
 *   放入链接脚本定义的 ".auto_init" 段。
 *   系统启动时 fw_auto_init_run() 遍历段内所有条目并调用。
 *
 * IAR 链接脚本需添加:
 *   place in ROM_region { section .auto_init };
 *
 * GCC 链接脚本需添加:
 *   .auto_init : { _s_auto_init = .; *(.auto_init) _e_auto_init = .; } > FLASH
 */
#ifndef FRAMEWORK_CORE_AUTO_INIT_H
#define FRAMEWORK_CORE_AUTO_INIT_H

#include "compiler.h"
#include <stdint.h>

/*============================================================================
 * 自动注册条目类型
 *============================================================================*/

/** 初始化优先级 — 控制注册顺序 */
typedef enum {
    FW_PRIO_EARLY   = 0,  /* 最早: 时钟、内存等基础设施 */
    FW_PRIO_HAL     = 1,  /* HAL层: GPIO/ADC/SPI等外设 */
    FW_PRIO_DRIVER  = 2,  /* 驱动层: LCD/SD卡等设备驱动 */
    FW_PRIO_SERVICE = 3,  /* 服务层: 日志/事件/内存池 */
    FW_PRIO_APP     = 4,  /* 应用层: 游戏/UI等 */
    FW_PRIO_LATE    = 5,  /* 最晚: 自检/监控 */
    FW_PRIO_COUNT
} fw_init_prio_t;

/** 自动注册条目结构 */
typedef struct {
    const char       *name;      /**< 模块名（编译期字符串） */
    void           (*init)(void); /**< 初始化函数指针 */
    fw_init_prio_t   prio;       /**< 优先级 */
    uint8_t          enabled;    /**< 是否启用（可被宏开关控制） */
} fw_init_entry_t;

/*============================================================================
 * IAR 自定义段定义
 *============================================================================*/
#if COMPILER_IAR

/* 声明只读段 .auto_init 用于存放注册表 */
#pragma section="auto_init"

/* 将注册条目放入 .auto_init 段 */
#define FW_SECTION_NAME  auto_init

#define FW_AUTO_ENTRY(name_str, init_fn, prio_val, en) \
    FW_ROOT const fw_init_entry_t \
    name_str##_auto_entry FW_PLACE_ROM(auto_init) = { \
        .name    = #name_str, \
        .init    = (init_fn), \
        .prio    = (prio_val), \
        .enabled = (en), \
    }

#elif COMPILER_GCC

/* GCC: 使用 section 属性 + used 防止消除 */
#define FW_AUTO_ENTRY(name_str, init_fn, prio_val, en) \
    __attribute__((used, section(".auto_init"))) \
    static const fw_init_entry_t \
    name_str##_auto_entry = { \
        .name    = #name_str, \
        .init    = (init_fn), \
        .prio    = (prio_val), \
        .enabled = (en), \
    }

#endif

/*============================================================================
 * 注册宏 — 模块使用这些宏即可自动注册
 *============================================================================*/

/**
 * 注册模块初始化函数
 * @param name  模块标识名（C标识符，用于生成变量名）
 * @param fn    初始化函数 void fn(void)
 * @param prio  优先级 fw_init_prio_t
 *
 * 用法:
 *   static void my_driver_init(void) { ... }
 *   FW_AUTO_REGISTER(my_driver, my_driver_init, FW_PRIO_DRIVER);
 */
#define FW_AUTO_REGISTER(name, fn, prio) \
    FW_AUTO_ENTRY(name, fn, prio, 1)

/**
 * 条件注册 — 受功能宏开关控制
 * @param name    模块名
 * @param fn      初始化函数
 * @param prio    优先级
 * @param cond    条件 (1=启用, 0=禁用)
 *
 * 用法:
 *   FW_AUTO_REGISTER_COND(lcd, lcd_init, FW_PRIO_DRIVER, FEATURE_LCD);
 */
#define FW_AUTO_REGISTER_COND(name, fn, prio, cond) \
    FW_AUTO_ENTRY(name, fn, prio, (cond) ? 1 : 0)

/*============================================================================
 * 遍历执行 — 系统启动时调用
 *============================================================================*/

#if COMPILER_IAR
/* IAR: 使用 __section_begin/end 获取段边界 */
#define FW_AUTO_INIT_FOREACH(callback) do { \
    const fw_init_entry_t *p = (const fw_init_entry_t *)__section_begin("auto_init"); \
    const fw_init_entry_t *e = (const fw_init_entry_t *)__section_end("auto_init"); \
    for (; p < e; p++) { \
        if (p->enabled) callback(p); \
    } \
} while(0)

#elif COMPILER_GCC
/* GCC: 使用 extern linker symbol */
extern const fw_init_entry_t _s_auto_init[];
extern const fw_init_entry_t _e_auto_init[];

#define FW_AUTO_INIT_FOREACH(callback) do { \
    const fw_init_entry_t *p = _s_auto_init; \
    const fw_init_entry_t *e = _e_auto_init; \
    for (; p < e; p++) { \
        if (p->enabled) callback(p); \
    } \
} while(0)

#endif

/**
 * 按优先级顺序执行所有已注册的初始化函数
 * 在 main() 早期阶段调用
 */
static inline void fw_auto_init_run(void)
{
    int prio;
    for (prio = 0; prio < FW_PRIO_COUNT; prio++) {
        FW_AUTO_INIT_FOREACH(^(const fw_init_entry_t *e) {
            if (e->prio == prio && e->enabled) {
                e->init();
            }
        });
    }
}

/* C90兼容: 内联函数改为宏实现（IAR默认C90） */
#define FW_AUTO_INIT_RUN() do { \
    static const char *_prio_names[] = { \
        "EARLY", "HAL", "DRIVER", "SERVICE", "APP", "LATE" \
    }; \
    int _prio; \
    for (_prio = 0; _prio < FW_PRIO_COUNT; _prio++) { \
        FW_AUTO_INIT_FOREACH(&(void)); \
    } \
} while(0)

/* 简化版: 不按优先级，直接遍历（兼容C90无block） */
#define FW_AUTO_INIT_RUN_ALL() do { \
    const fw_init_entry_t *_p; \
    const fw_init_entry_t *_e; \
    FW_AUTO_INIT_GET_BOUNDS(_p, _e); \
    while (_p < _e) { \
        if (_p->enabled && _p->init) { \
            _p->init(); \
        } \
        _p++; \
    } \
} while(0)

#if COMPILER_IAR
#define FW_AUTO_INIT_GET_BOUNDS(p, e) do { \
    p = (const fw_init_entry_t *)__section_begin("auto_init"); \
    e = (const fw_init_entry_t *)__section_end("auto_init"); \
} while(0)
#elif COMPILER_GCC
#define FW_AUTO_INIT_GET_BOUNDS(p, e) do { \
    p = _s_auto_init; \
    e = _e_auto_init; \
} while(0)
#endif

/*============================================================================
 * 按优先级分批执行
 *============================================================================*/
#define FW_AUTO_INIT_RUN_PRIO(target_prio) do { \
    const fw_init_entry_t *_p; \
    const fw_init_entry_t *_e; \
    FW_AUTO_INIT_GET_BOUNDS(_p, _e); \
    while (_p < _e) { \
        if (_p->enabled && _p->init && _p->prio == (target_prio)) { \
            _p->init(); \
        } \
        _p++; \
    } \
} while(0)

/**
 * 完整初始化序列 — 按优先级从早到晚依次执行
 */
#define FW_AUTO_INIT_SYSTEM() do { \
    FW_AUTO_INIT_RUN_PRIO(FW_PRIO_EARLY); \
    FW_AUTO_INIT_RUN_PRIO(FW_PRIO_HAL); \
    FW_AUTO_INIT_RUN_PRIO(FW_PRIO_DRIVER); \
    FW_AUTO_INIT_RUN_PRIO(FW_PRIO_SERVICE); \
    FW_AUTO_INIT_RUN_PRIO(FW_PRIO_APP); \
    FW_AUTO_INIT_RUN_PRIO(FW_PRIO_LATE); \
} while(0)

/*============================================================================
 * 通用自动注册表 — 可用于驱动/游戏/命令等任意类型
 *============================================================================*/

/**
 * FW_AUTO_TABLE(section, type) — 创建自定义注册表
 * 不同类型模块可使用不同段，互不干扰
 */
#define FW_AUTO_TABLE_DECLARE(table_name, entry_type) \
    typedef entry_type table_name##_entry_t

#if COMPILER_IAR
  #define FW_AUTO_TABLE_ENTRY(table_name, var_name, entry_type, ...) \
      FW_ROOT const entry_type var_name FW_PLACE_ROM(table_name) = __VA_ARGS__
  #pragma section="drv_table"
  #pragma section="game_table"
  #pragma section="cmd_table"
#elif COMPILER_GCC
  #define FW_AUTO_TABLE_ENTRY(table_name, var_name, entry_type, ...) \
      __attribute__((used, section("." #table_name))) \
      static const entry_type var_name = __VA_ARGS__
#endif

#endif /* FRAMEWORK_CORE_AUTO_INIT_H */
