/**
 * @file mempool.h
 * @brief 分层静态内存池 — 彻底取代 malloc/free 动态分配
 *
 * 目标5: 静态内存管理 (消灭动态分配)
 *   所有内存池由调用方提供静态后备内存，零运行期堆开销、零碎片、O(1) 分配/释放。
 *
 * 目标1: 编译期全量优化校验
 *   通过 FW_STATIC_ASSERT 在编译期固化 block_size >= sizeof(void*) 约束。
 *
 * 目标3: 彻底消除模块全局耦合
 *   池句柄 fw_mempool_t 为不透明类型，状态封装于 .c 文件，跨文件仅传句柄。
 *
 * 设计要点:
 *   - 空闲块内部以单向链表串联，next 指针复用块首 sizeof(void*) 字节，
 *     分配后整块归用户使用，零元数据开销 (关闭 FW_MEMPOOL_GUARD 时)。
 *   - 可选 FW_MEMPOOL_GUARD 在每块前后插入魔数，用于溢出/双重释放检测。
 *   - 统计计数使用 FW_VOLATILE，可在中断/主循环共享读取。
 */
#ifndef FRAMEWORK_MEM_MEMPOOL_H
#define FRAMEWORK_MEM_MEMPOOL_H

#include "compiler.h"
#include <stdint.h>

/*============================================================================
 * 配置开关
 *============================================================================*/

/** 溢出检测守卫: 1=在每个块前后插入魔数, 0=零开销模式 */
#ifndef FW_MEMPOOL_GUARD
  #define FW_MEMPOOL_GUARD  0
#endif

/** 系统允许同时存在的内存池数量 (池描述符本身也从静态数组分配) */
#ifndef FW_MEMPOOL_MAX_POOLS
  #define FW_MEMPOOL_MAX_POOLS  8
#endif

/*============================================================================
 * 守卫常量 — 关闭时开销为 0
 *============================================================================*/
#if FW_MEMPOOL_GUARD
  #define FW_MEMPOOL_GUARD_HEAD      4u
  #define FW_MEMPOOL_GUARD_TAIL      4u
  #define FW_MEMPOOL_GUARD_OVERHEAD  (FW_MEMPOOL_GUARD_HEAD + FW_MEMPOOL_GUARD_TAIL)
  #define FW_MEMPOOL_GUARD_MAGIC     0xA5A5A5A5UL
#else
  #define FW_MEMPOOL_GUARD_HEAD      0u
  #define FW_MEMPOOL_GUARD_TAIL      0u
  #define FW_MEMPOOL_GUARD_OVERHEAD  0u
  #define FW_MEMPOOL_GUARD_MAGIC     0u
#endif

/*============================================================================
 * 不透明池类型 — 内部结构仅在 mempool.c 中定义
 *============================================================================*/
FW_OPAQUE_TYPE(fw_mempool);

/*============================================================================
 * 统计查询类型
 *============================================================================*/
#define FW_MEMPOOL_STAT_FREE   0u   /**< 查询空闲块数 */
#define FW_MEMPOOL_STAT_USED   1u   /**< 查询已用块数 */
#define FW_MEMPOOL_STAT_PEAK   2u   /**< 查询历史峰值用量 */

/*============================================================================
 * 公共 API
 *============================================================================*/

/**
 * 从静态后备内存创建一个内存池
 * @param block_size   单块可用字节数 (必须 >= sizeof(void*))
 * @param block_count  块数量 (必须 > 0)
 * @param backing_mem  调用方提供的静态后备内存, 容量需 >=
 *                     (FW_ALIGN_UP(block_size,sizeof(void*)) + FW_MEMPOOL_GUARD_OVERHEAD)
 *                     * block_count 字节, 且按 sizeof(void*) 对齐
 * @return 池句柄, 失败返回 NULL
 */
fw_mempool_t *fw_mempool_create(uint32_t block_size,
                                uint32_t block_count,
                                void     *backing_mem);

/**
 * 分配一个块
 * @return 块首地址, 池耗尽返回 NULL
 */
void *fw_mempool_alloc(fw_mempool_t *pool);

/**
 * 释放块回池
 * @return 0=成功, -1=ptr 不属于该池或守卫校验失败
 */
int fw_mempool_free(fw_mempool_t *pool, void *ptr);

/**
 * 查询统计信息
 * @param stat_type  FW_MEMPOOL_STAT_FREE / _USED / _PEAK
 */
uint32_t fw_mempool_stats(fw_mempool_t *pool, uint8_t stat_type);

/*============================================================================
 * 便捷内联访问器 — static inline, IAR 支持, 编译期可优化为零调用
 *============================================================================*/
static inline uint32_t fw_mempool_free_count(fw_mempool_t *pool)
{
    return fw_mempool_stats(pool, FW_MEMPOOL_STAT_FREE);
}

static inline uint32_t fw_mempool_used_count(fw_mempool_t *pool)
{
    return fw_mempool_stats(pool, FW_MEMPOOL_STAT_USED);
}

static inline uint32_t fw_mempool_peak_usage(fw_mempool_t *pool)
{
    return fw_mempool_stats(pool, FW_MEMPOOL_STAT_PEAK);
}

/*============================================================================
 * 静态池定义宏 — 编译期已知尺寸时使用, 触发 FW_STATIC_ASSERT 校验
 *
 * 用法 (文件作用域):
 *   FW_MEMPOOL_DEFINE(my_pool, 32, 8);
 *   void *p = fw_mempool_alloc(my_pool());
 *
 * 注: 展开为多条声明, 末尾以 typedef 标记结束, 故调用处需带分号。
 *============================================================================*/
#define FW_MEMPOOL_DEFINE(name, blk_size, blk_count) \
    /* 编译期断言: 块必须能容纳一个链表 next 指针 (msg=池名, 保证可多次使用) */ \
    FW_STATIC_ASSERT((blk_size) >= (uint32_t)sizeof(void *), name); \
    /* 静态后备内存, 用 union 强制按指针宽度对齐 */ \
    static union name##_storage_u { \
        unsigned char bytes[ ((FW_ALIGN_UP((blk_size), (uint32_t)sizeof(void *))) \
                              + FW_MEMPOOL_GUARD_OVERHEAD) * (blk_count) ]; \
        void *align; \
    } name##_storage; \
    /* 惰性初始化的池句柄访问器 */ \
    static fw_mempool_t *name(void) { \
        static fw_mempool_t *_p = 0; \
        if (!_p) { \
            _p = fw_mempool_create((blk_size), (blk_count), \
                                   (void *)name##_storage.bytes); \
        } \
        return _p; \
    } \
    typedef int __fw_mempool_end_##name

#endif /* FRAMEWORK_MEM_MEMPOOL_H */
