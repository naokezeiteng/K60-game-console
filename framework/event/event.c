/**
 * @file event.c
 * @brief 中断到主循环异步事件系统实现
 *
 * 目标8: 中断到主循环异步事件系统
 * 目标2: 自定义段自动注册架构 — fw_event_init 自动注册
 * 目标3: 彻底消除模块全局耦合 — 队列/处理器表为文件内 static, 无 extern 全局
 *
 * 实现:
 *   - 队列: 环形缓冲区 (单生产者 ISR / 单消费者 main)。
 *   - 派发: evt_id -> handler 查找表, 容量 FW_EVENT_MAX_HANDLERS。
 *   - 索引/计数 FW_VOLATILE, ARM 32 位访问原子。
 *   - fw_event_process 排空"当前快照"事件后派发, 避免新事件导致活锁。
 */
#include "event.h"
#include "compiler.h"
#include "auto_init.h"
#include <stdint.h>

/*============================================================================
 * 处理器表条目类型
 *============================================================================*/
typedef struct {
    uint16_t           evt_id;
    fw_event_handler_t handler;
} fw_event_handler_entry_t;

/*============================================================================
 * 模块私有状态 — 文件内 static, 对外不可见 (无 extern 全局变量)
 *============================================================================*/
static fw_event_queue_t         s_queue;
static fw_event_handler_entry_t s_handlers[FW_EVENT_MAX_HANDLERS];
static FW_VOLATILE uint8_t      s_handler_count;

/*============================================================================
 * fw_event_init — 清空队列与处理器表
 *============================================================================*/
void fw_event_init(void)
{
    uint8_t i;

    s_queue.head  = 0;
    s_queue.tail  = 0;
    s_queue.count = 0;
    s_handler_count = 0;
    for (i = 0; i < FW_EVENT_MAX_HANDLERS; i++) {
        s_handlers[i].evt_id  = 0;
        s_handlers[i].handler = 0;
    }
}

/*============================================================================
 * fw_event_subscribe — 注册/覆盖处理器
 *============================================================================*/
int fw_event_subscribe(uint16_t evt_id, fw_event_handler_t handler)
{
    uint8_t i;
    uint8_t cnt;

    if (handler == 0) { return -1; }

    cnt = s_handler_count;

    /* 同类事件已存在则覆盖 */
    for (i = 0; i < cnt; i++) {
        if (s_handlers[i].evt_id == evt_id) {
            s_handlers[i].handler = handler;
            return 0;
        }
    }

    if (cnt >= FW_EVENT_MAX_HANDLERS) { return -1; }

    s_handlers[cnt].evt_id  = evt_id;
    s_handlers[cnt].handler = handler;
    s_handler_count = (uint8_t)(cnt + 1);
    return 0;
}

/*============================================================================
 * fw_event_post_from_isr — 非阻塞入队 (ISR 安全)
 *============================================================================*/
int fw_event_post_from_isr(uint16_t evt_id, void *data)
{
    uint16_t tail;

    if (s_queue.count >= EVENT_QUEUE_SIZE) { return -1; }   /* 队列满 */

    tail = s_queue.tail;
    s_queue.buf[tail].event_id = evt_id;
    s_queue.buf[tail].data     = data;
    s_queue.buf[tail].next     = 0;

    tail++;
    if (tail >= EVENT_QUEUE_SIZE) { tail = 0; }
    s_queue.tail  = tail;
    s_queue.count = (uint16_t)(s_queue.count + 1u);
    return 0;
}

/*============================================================================
 * fw_event_post — 主循环上下文投递, 复用 ISR 入队逻辑
 *============================================================================*/
int fw_event_post(uint16_t evt_id, void *data)
{
    return fw_event_post_from_isr(evt_id, data);
}

/*============================================================================
 * fw_event_process — 主循环派发: 排空当前事件快照
 *============================================================================*/
void fw_event_process(void)
{
    uint16_t snap;            /* 进入时事件数快照, 防止新入队事件导致活锁 */
    uint16_t evt_id;
    void    *data;
    uint8_t  i;
    uint8_t  cnt;
    fw_event_handler_t h;

    snap = s_queue.count;
    while (snap > 0) {
        /* 取队首 */
        evt_id = s_queue.buf[s_queue.head].event_id;
        data   = s_queue.buf[s_queue.head].data;

        /* 推进 head */
        {
            uint16_t head = s_queue.head;
            head++;
            if (head >= EVENT_QUEUE_SIZE) { head = 0; }
            s_queue.head = head;
        }
        s_queue.count = (uint16_t)(s_queue.count - 1u);
        snap--;

        /* 派发给所有匹配处理器 */
        cnt = s_handler_count;
        for (i = 0; i < cnt; i++) {
            if (s_handlers[i].evt_id == evt_id &&
                s_handlers[i].handler != 0) {
                h = s_handlers[i].handler;
                h(evt_id, data);
            }
        }
    }
}

/*============================================================================
 * 自动注册 — SERVICE 优先级, 在日志/内存池之后、应用之前初始化
 *============================================================================*/
FW_AUTO_REGISTER(event, fw_event_init, FW_PRIO_SERVICE);
