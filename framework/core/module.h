/**
 * @file module.h
 * @brief 模块句柄化管理 — 彻底消除跨文件extern全局变量
 *
 * 目标3: 彻底消除模块全局耦合
 * 采用不透明结构体 + 模块静态私有化 + 实例句柄化管理，
 * 每个模块状态封装在独立结构体中，通过句柄传递，杜绝extern。
 */
#ifndef FRAMEWORK_CORE_MODULE_H
#define FRAMEWORK_CORE_MODULE_H

#include "compiler.h"
#include "auto_init.h"
#include <stdint.h>
#include <string.h>

/*============================================================================
 * 模块实例管理 — 句柄化，支持多实例
 *============================================================================*/

/**
 * 模块实例基类 — 所有模块实例结构包含此头部
 * 模块定义自己的 fw_module_t 子类，通过 ctx 传递
 */
typedef struct fw_module {
    const char       *name;     /**< 模块名 */
    uint8_t           id;       /**< 模块ID（自动分配） */
    uint8_t           active;   /**< 是否激活 */
} fw_module_t;

/** 模块注册表条目 — 用于自动注册到游戏/设备管理器 */
typedef struct {
    const void         *ops;     /**< 操作表指针 */
    const char         *name;    /**< 模块显示名 */
    uint8_t             id;      /**< 模块ID */
} fw_module_reg_t;

/*============================================================================
 * 模块私有化宏 — 将全局变量封装为模块静态私有
 *============================================================================*/

/**
 * FW_MODULE_PRIV — 声明模块私有状态
 * 将原本散落的全局变量封装进一个结构体，仅在模块 .c 文件中可见
 *
 * 用法:
 *   FW_MODULE_PRIV(lcd, {
 *       uint8_t backlight;
 *       uint8_t contrast;
 *   })
 * 生成:
 *   static struct lcd_priv_s { uint8_t backlight; uint8_t contrast; } lcd_priv;
 *   #define LCD_PRIV()  (&lcd_priv)
 */
#define FW_MODULE_PRIV(name, fields) \
    struct name##_priv_s fields; \
    static struct name##_priv_s name##_priv; \
    static inline struct name##_priv_s *name##_priv_get(void) { return &name##_priv; }

/**
 * FW_MODULE_INST — 多实例支持
 * 与 FW_MODULE_PRIV 不同，创建可传递的实例而非单例
 */
#define FW_MODULE_INST(name, fields) \
    typedef struct name##_inst_s fields name##_inst_t

/*============================================================================
 * 模块初始化/反初始化标准接口
 *============================================================================*/

/** 模块初始化函数签名 */
typedef int (*fw_module_init_fn)(void);
typedef int (*fw_module_deinit_fn)(void);

/** 模块描述符 — 自动注册到系统 */
typedef struct {
    const char           *name;
    fw_module_init_fn     init;
    fw_module_deinit_fn   deinit;
    fw_init_prio_t        prio;
    uint8_t               enabled;
} fw_module_desc_t;

/**
 * 注册模块 — 自动放入 .auto_init 段
 * 系统启动时按优先级自动调用 init
 */
#define FW_MODULE_REGISTER(name, init_fn, deinit_fn, prio) \
    static int name##_init(void) { init_fn(); return 0; } \
    static int name##_deinit(void) { deinit_fn(); return 0; } \
    FW_ROOT const fw_module_desc_t name##_desc FW_PLACE_ROM(auto_init) = { \
        .name    = #name, \
        .init    = name##_init, \
        .deinit  = name##_deinit, \
        .prio    = (prio), \
        .enabled = 1, \
    }

/**
 * 条件注册模块 — 受功能宏开关控制
 */
#define FW_MODULE_REGISTER_COND(name, init_fn, deinit_fn, prio, cond) \
    static int name##_init(void) { if (cond) { init_fn(); } return 0; } \
    static int name##_deinit(void) { if (cond) { deinit_fn(); } return 0; } \
    FW_ROOT const fw_module_desc_t name##_desc FW_PLACE_ROM(auto_init) = { \
        .name    = #name, \
        .init    = name##_init, \
        .deinit  = name##_deinit, \
        .prio    = (prio), \
        .enabled = (cond) ? 1 : 0, \
    }

/*============================================================================
 * 游戏模块自动注册表 — 独立段
 *============================================================================*/

#if COMPILER_IAR
  #pragma section="game_table"
#elif COMPILER_GCC
  extern const fw_game_ops_t _s_game_table[];
  extern const fw_game_ops_t _e_game_table[];
#endif

/** 游戏注册条目 — 放入 game_table 段 */
typedef struct {
    const fw_game_ops_t *ops;
    const char          *name;
} fw_game_reg_t;

/**
 * 注册游戏 — 自动加入游戏列表
 * 游戏管理器遍历 game_table 段即可列出所有游戏
 */
#if COMPILER_IAR
  #define FW_GAME_REGISTER(name, ops_ptr) \
      FW_ROOT const fw_game_reg_t name##_game_reg FW_PLACE_ROM(game_table) = { \
          .ops  = (ops_ptr), \
          .name = #name, \
      }
#elif COMPILER_GCC
  #define FW_GAME_REGISTER(name, ops_ptr) \
      __attribute__((used, section(".game_table"))) \
      static const fw_game_reg_t name##_game_reg = { \
          .ops  = (ops_ptr), \
          .name = #name, \
      }
#endif

/** 遍历所有已注册游戏 */
#if COMPILER_IAR
  #define FW_GAME_FOREACH(var) \
      const fw_game_reg_t *var = (const fw_game_reg_t *)__section_begin("game_table"); \
      const fw_game_reg_t *var##_end = (const fw_game_reg_t *)__section_end("game_table"); \
      for (; var < var##_end; var++)
#elif COMPILER_GCC
  #define FW_GAME_FOREACH(var) \
      const fw_game_reg_t *var = (const fw_game_reg_t *)_s_game_table; \
      const fw_game_reg_t *var##_end = (const fw_game_reg_t *)_e_game_table; \
      for (; var < var##_end; var++)
#endif

#endif /* FRAMEWORK_CORE_MODULE_H */
