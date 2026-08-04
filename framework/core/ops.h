/**
 * @file ops.h
 * @brief 零开销面向对象抽象 — 操作表(OPS)实现纯C多态
 *
 * 目标4: 零开销面向对象抽象
 * 基于函数指针表实现接口标准化，零运行时额外开销（仅一次指针解引用）。
 * 搭配不透明句柄，实现模块状态隔离与多实例支持。
 */
#ifndef FRAMEWORK_CORE_OPS_H
#define FRAMEWORK_CORE_OPS_H

#include "compiler.h"
#include <stdint.h>

/*============================================================================
 * OPS 基础结构 — 所有操作表的公共头部
 *============================================================================*/

/** 操作表公共头部 — 用于运行时类型检查 */
#define FW_OPS_HEAD \
    const char *name;        /**< 操作表名（编译期字符串） */ \
    uint16_t    version;     /**< 接口版本号 */ \
    uint16_t    size;        /**< 操作表结构体大小（编译期校验） */

/** 操作表类型安全检查宏 */
#define FW_OPS_NAME(ops)       ((ops)->name)
#define FW_OPS_VERSION(ops)    ((ops)->version)
#define FW_OPS_CHECK(ops, expected_name) \
    (FW_OPS_NAME(ops) && (FW_STRING_EQ(FW_OPS_NAME(ops), expected_name)))

#define FW_STRING_EQ(a, b)  (strcmp((a), (b)) == 0)

/*============================================================================
 * 设备驱动 OPS 模板 — HAL层标准接口
 *============================================================================*/

/** 设备状态 */
typedef enum {
    FW_DEV_STATE_CLOSED = 0,
    FW_DEV_STATE_OPEN   = 1,
    FW_DEV_STATE_BUSY   = 2,
    FW_DEV_STATE_ERROR  = 3,
} fw_dev_state_t;

/** 设备操作表 — 所有HAL设备驱动实现此接口 */
typedef struct fw_dev_ops {
    FW_OPS_HEAD;
    int  (*open)   (void *ctx);
    int  (*close)  (void *ctx);
    int  (*read)   (void *ctx, uint8_t *buf, uint32_t len);
    int  (*write)  (void *ctx, const uint8_t *buf, uint32_t len);
    int  (*ioctl)  (void *ctx, uint32_t cmd, uint32_t arg);
} fw_dev_ops_t;

/** 设备实例 — 句柄化，封装操作表+私有上下文 */
typedef struct fw_device {
    const fw_dev_ops_t *ops;   /**< 操作表指针（多态核心） */
    void               *ctx;   /**< 私有上下文（不透明） */
    fw_dev_state_t      state; /**< 当前状态 */
} fw_device_t;

/**
 * 设备操作宏 — 零开销内联调用，编译器可优化
 * 使用前不做NULL检查（调用者保证ops有效），零额外开销
 */
#define FW_DEV_OPEN(dev)        ((dev)->ops->open((dev)->ctx))
#define FW_DEV_CLOSE(dev)       ((dev)->ops->close((dev)->ctx))
#define FW_DEV_READ(dev, b, l)  ((dev)->ops->read((dev)->ctx, (b), (l)))
#define FW_DEV_WRITE(dev, b, l) ((dev)->ops->write((dev)->ctx, (b), (l)))
#define FW_DEV_IOCTL(dev, c, a) ((dev)->ops->ioctl((dev)->ctx, (c), (a)))

/** 安全版（带NULL检查，用于不可信输入） */
#define FW_DEV_OPEN_S(dev) \
    ((dev) && (dev)->ops && (dev)->ops->open ? FW_DEV_OPEN(dev) : -1)

/*============================================================================
 * 游戏 OPS 模板 — 应用层标准接口
 *============================================================================*/

/** 游戏事件类型 */
typedef enum {
    FW_GAME_EVT_NONE    = 0,
    FW_GAME_EVT_UP      = 1,
    FW_GAME_EVT_DOWN    = 2,
    FW_GAME_EVT_LEFT    = 3,
    FW_GAME_EVT_RIGHT   = 4,
    FW_GAME_EVT_CONFIRM = 5,
    FW_GAME_EVT_CANCEL  = 6,
    FW_GAME_EVT_TICK    = 7,
} fw_game_evt_t;

/** 游戏状态 */
typedef enum {
    FW_GAME_STATE_IDLE    = 0,
    FW_GAME_STATE_RUNNING = 1,
    FW_GAME_STATE_PAUSED  = 2,
    FW_GAME_STATE_WIN     = 3,
    FW_GAME_STATE_LOSE    = 4,
} fw_game_state_t;

/** 游戏操作表 — 所有游戏实现此接口 */
typedef struct fw_game_ops {
    FW_OPS_HEAD;
    const char  *title;                        /**< 显示标题 */
    void       (*on_start)(void *ctx);         /**< 游戏开始 */
    void       (*on_stop) (void *ctx);         /**< 游戏停止 */
    void       (*on_event)(void *ctx, fw_game_evt_t evt); /**< 输入事件 */
    void       (*on_tick) (void *ctx);         /**< 周期刷新 */
    fw_game_state_t (*get_state)(void *ctx);   /**< 查询状态 */
} fw_game_ops_t;

/** 游戏实例 — 句柄化 */
typedef struct fw_game {
    const fw_game_ops_t *ops;
    void                *ctx;
    fw_game_state_t      state;
} fw_game_t;

#define FW_GAME_START(g)     ((g)->ops->on_start((g)->ctx))
#define FW_GAME_STOP(g)      ((g)->ops->on_stop((g)->ctx))
#define FW_GAME_EVENT(g, e)  ((g)->ops->on_event((g)->ctx, (e)))
#define FW_GAME_TICK(g)      ((g)->ops->on_tick((g)->ctx))
#define FW_GAME_STATE(g)     ((g)->ops->get_state((g)->ctx))

/*============================================================================
 * OPS 定义辅助宏 — 简化操作表声明
 *============================================================================*/

/** 声明一个设备操作表实例 */
#define FW_DEV_OPS_DEFINE(name, ...) \
    const fw_dev_ops_t name = { \
        .name    = #name, \
        .version = 1, \
        .size    = sizeof(fw_dev_ops_t), \
        __VA_ARGS__ \
    }

/** 声明一个游戏操作表实例 */
#define FW_GAME_OPS_DEFINE(name, game_title, ...) \
    const fw_game_ops_t name = { \
        .name    = #name, \
        .version = 1, \
        .size    = sizeof(fw_game_ops_t), \
        .title   = (game_title), \
        __VA_ARGS__ \
    }

/*============================================================================
 * 查表式状态机框架
 *============================================================================*/

/** 状态机事件-动作表条目 */
typedef struct {
    uint8_t  cur_state;   /**< 当前状态 */
    uint8_t  event;       /**< 触发事件 */
    uint8_t  next_state;  /**< 转移状态 */
    void   (*action)(void *ctx); /**< 执行动作（可为NULL） */
} fw_state_row_t;

/** 状态机定义 */
typedef struct {
    const fw_state_row_t *table;  /**< 状态转移表 */
    uint16_t              n_rows; /**< 表行数 */
    uint8_t               state;  /**< 当前状态 */
} fw_fsm_t;

/**
 * 状态机处理事件 — 查表式，O(1)~O(n)取决于表大小
 * @return 1=状态转移发生, 0=无匹配
 */
static inline int fw_fsm_handle(fw_fsm_t *fsm, uint8_t event, void *ctx)
{
    uint16_t i;
    for (i = 0; i < fsm->n_rows; i++) {
        if (fsm->table[i].cur_state == fsm->state &&
            fsm->table[i].event == event) {
            fsm->state = fsm->table[i].next_state;
            if (fsm->table[i].action) {
                fsm->table[i].action(ctx);
            }
            return 1;
        }
    }
    return 0;
}

/** 定义状态机表 */
#define FW_FSM_TABLE(name) \
    static const fw_state_row_t name[]

/** 获取状态机表行数 */
#define FW_FSM_TABLE_SIZE(name) \
    (sizeof(name) / sizeof(name[0]))

#endif /* FRAMEWORK_CORE_OPS_H */
