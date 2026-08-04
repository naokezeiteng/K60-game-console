/**
 * @file event.h
 * @brief 中断到主循环异步事件系统 — ISR 延迟处理
 *
 * 目标8: 中断到主循环异步事件系统
 *   ISR 内通过环形缓冲区投递事件 (非阻塞), 主循环 fw_event_process() 集中派发,
 *   把"硬实时 ISR"与"非实时业务逻辑"彻底解耦。
 *
 * 目标2: 自定义段自动注册架构
 *   fw_event_init() 经 FW_AUTO_REGISTER 自动注册。
 *
 * 目标3: 彻底消除模块全局耦合
 *   队列/处理器表封装为模块私有 (FW_MODULE_PRIV), 对外仅暴露句柄与 API。
 *
 * 线程模型:
 *   - ISR 仅写环形缓冲区 (post_from_isr), 主循环仅读+派发 (process)。
 *   - 共享索引/计数全部 FW_VOLATILE, ARM 32 位读写原子, 单生产者-单消费者安全。
 */
#ifndef FRAMEWORK_EVENT_EVENT_H
#define FRAMEWORK_EVENT_EVENT_H

#include "compiler.h"
#include <stdint.h>

/*============================================================================
 * 配置
 *============================================================================*/

/** 环形缓冲区容量 (2 的幂时取模可优化为掩码, 但代码对任意值均正确) */
#ifndef EVENT_QUEUE_SIZE
  #define EVENT_QUEUE_SIZE  16
#endif

/** 单类事件最多注册的处理器数量 */
#ifndef FW_EVENT_MAX_HANDLERS
  #define FW_EVENT_MAX_HANDLERS  8
#endif

/*============================================================================
 * 事件节点
 *   next 字段保留, 供链表式队列/空闲链复用; 当前实现采用环形缓冲区
 *============================================================================*/
typedef struct fw_event_s {
    uint16_t            event_id;   /* 事件类型 ID */
    void               *data;      /* 事件附加数据 (指针/句柄) */
    struct fw_event_s  *next;      /* 保留: 链表式扩展用 */
} fw_event_t;

/*============================================================================
 * 事件队列 — 环形缓冲区实现
 *   所有索引/计数 FW_VOLATILE, 供 ISR 与主循环并发访问
 *============================================================================*/
typedef struct fw_event_queue_s {
    fw_event_t           buf[EVENT_QUEUE_SIZE];
    FW_VOLATILE uint16_t head;     /* 出队索引 (主循环消费) */
    FW_VOLATILE uint16_t tail;     /* 入队索引 (ISR 生产) */
    FW_VOLATILE uint16_t count;    /* 当前待处理事件数 */
} fw_event_queue_t;

/*============================================================================
 * 事件处理器签名
 *============================================================================*/
typedef void (*fw_event_handler_t)(uint16_t evt_id, void *data);

/*============================================================================
 * 编译期事件 ID 定义宏
 *
 * 用法:
 *   enum app_events {
 *       FW_EVENT_DEFINE(EVT_KEY),
 *       FW_EVENT_DEFINE(EVT_TICK),
 *   };
 * 展开为枚举常量, 编译期固化, switch 可穷举校验。
 *============================================================================*/
#define FW_EVENT_DEFINE(id)  id

/*============================================================================
 * 公共 API
 *============================================================================*/

/** 初始化事件子系统 (由 FW_AUTO_REGISTER 自动调用) */
void fw_event_init(void);

/**
 * 从 ISR 投递事件 — 非阻塞, 写环形缓冲区
 * @return 0=成功, -1=队列满
 */
int fw_event_post_from_isr(uint16_t evt_id, void *data);

/**
 * 从主循环上下文投递事件
 * @return 0=成功, -1=队列满
 */
int fw_event_post(uint16_t evt_id, void *data);

/**
 * 主循环调用: 排空当前待处理事件并派发给已注册处理器
 * 注意: 处理器内不可阻塞过久, 亦不应再次 post 大量事件导致活锁
 */
void fw_event_process(void);

/**
 * 订阅事件 — 为指定 evt_id 注册处理器 (同类重复订阅将覆盖旧处理器)
 * @return 0=成功, -1=处理器表满或 handler 为空
 */
int fw_event_subscribe(uint16_t evt_id, fw_event_handler_t handler);

#endif /* FRAMEWORK_EVENT_EVENT_H */
