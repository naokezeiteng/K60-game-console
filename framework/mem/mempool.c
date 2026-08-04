/**
 * @file mempool.c
 * @brief 分层静态内存池实现
 *
 * 目标5: 静态内存管理 (消灭动态分配)
 * 目标3: 彻底消除模块全局耦合 — 池状态不透明, 仅本文件可见
 *
 * 实现:
 *   - 池描述符从静态数组 s_pools 分配, 不依赖堆。
 *   - 空闲链表为单向链表, next 指针存于空闲块首部 sizeof(void*) 字节;
 *     块一旦分配, 整块交给用户, 无任何元数据残留 (零开销)。
 *   - FW_MEMPOOL_GUARD 开启时, 每块前后各 4 字节魔数用于溢出/双重释放检测。
 *   - 共享计数 (free/used/peak) 标记 FW_VOLATILE, 供中断与主循环并发读取。
 *
 * 块内存布局 (stride = ALIGN_UP(block_size, sizeof(void*)) + GUARD_OVERHEAD):
 *   [head_guard][user: block_size ...][align padding][tail_guard]
 *   user_ptr = block_start + GUARD_HEAD
 *   空闲时 user_ptr[0..sizeof(void*)] 存放下一个空闲块的 user_ptr。
 */
#include "mempool.h"
#include "compiler.h"

/*============================================================================
 * 池描述符 — 对应头文件中的不透明 fw_mempool_t
 *============================================================================*/
struct fw_mempool_s {
    uint8_t *base;              /* 后备内存基址 */
    uint32_t stride;            /* 相邻块间距 (含对齐与守卫开销) */
    uint32_t block_size;        /* 用户可见单块字节数 */
    uint32_t block_count;       /* 总块数 */
    FW_VOLATILE uint32_t free_count;  /* 当前空闲块数 */
    FW_VOLATILE uint32_t used_count;  /* 当前已用块数 */
    FW_VOLATILE uint32_t peak_used;   /* 历史峰值已用块数 */
    void    *free_list;         /* 空闲链表头 (指向首个空闲块 user_ptr) */
};

/*============================================================================
 * 池描述符静态池 — 取代 malloc 管理池对象本身
 *============================================================================*/
static struct fw_mempool_s s_pools[FW_MEMPOOL_MAX_POOLS];
static uint8_t s_pool_used[FW_MEMPOOL_MAX_POOLS];

/*============================================================================
 * 内部辅助: 写入/校验守卫魔数
 *============================================================================*/
#if FW_MEMPOOL_GUARD
static void guard_set(void *user_ptr, uint32_t stride)
{
    uint8_t *block_start = (uint8_t *)user_ptr - FW_MEMPOOL_GUARD_HEAD;
    uint32_t *head = (uint32_t *)block_start;
    uint32_t *tail = (uint32_t *)(block_start + stride - FW_MEMPOOL_GUARD_TAIL);
    *head = FW_MEMPOOL_GUARD_MAGIC;
    *tail = FW_MEMPOOL_GUARD_MAGIC;
}

/* 返回 0=校验通过, -1=魔数被破坏 */
static int guard_check(void *user_ptr, uint32_t stride)
{
    uint8_t *block_start = (uint8_t *)user_ptr - FW_MEMPOOL_GUARD_HEAD;
    uint32_t *head = (uint32_t *)block_start;
    uint32_t *tail = (uint32_t *)(block_start + stride - FW_MEMPOOL_GUARD_TAIL);
    if (*head != FW_MEMPOOL_GUARD_MAGIC) { return -1; }
    if (*tail != FW_MEMPOOL_GUARD_MAGIC) { return -1; }
    /* 抹除魔数, 便于发现双重释放 */
    *head = 0;
    *tail = 0;
    return 0;
}
#endif /* FW_MEMPOOL_GUARD */

/*============================================================================
 * fw_mempool_create
 *============================================================================*/
fw_mempool_t *fw_mempool_create(uint32_t block_size,
                                uint32_t block_count,
                                void     *backing_mem)
{
    uint32_t i;
    uint32_t align;
    uint32_t stride;
    uint8_t *user_base;
    struct fw_mempool_s *p;

    /* 参数校验: 运行期兜底 (编译期断言见 FW_MEMPOOL_DEFINE) */
    if (backing_mem == 0 || block_count == 0) { return 0; }
    if (block_size < (uint32_t)sizeof(void *)) { return 0; }

    /* 从静态池数组中取一个空闲描述符槽位 */
    p = 0;
    for (i = 0; i < FW_MEMPOOL_MAX_POOLS; i++) {
        if (!s_pool_used[i]) {
            p = &s_pools[i];
            s_pool_used[i] = 1;
            break;
        }
    }
    if (p == 0) { return 0; }

    align  = (uint32_t)sizeof(void *);
    stride = FW_ALIGN_UP(block_size, align) + FW_MEMPOOL_GUARD_OVERHEAD;

    p->base        = (uint8_t *)backing_mem;
    p->stride      = stride;
    p->block_size  = block_size;
    p->block_count = block_count;
    p->free_count  = block_count;
    p->used_count  = 0;
    p->peak_used   = 0;

    /* 构建空闲链表: 每个块的 user_ptr 首槽存放下一块 user_ptr */
    user_base = p->base + FW_MEMPOOL_GUARD_HEAD;
    p->free_list = user_base;
    for (i = 0; i < block_count; i++) {
        uint8_t *cur = user_base + (i * stride);
        void   **slot = (void **)cur;
        if ((i + 1) < block_count) {
            *slot = (void *)(cur + stride);
        } else {
            *slot = (void *)0;
        }
#if FW_MEMPOOL_GUARD
        guard_set((void *)cur, stride);
#endif
    }

    return (fw_mempool_t *)p;
}

/*============================================================================
 * fw_mempool_alloc — 从空闲链表头弹出一块
 *============================================================================*/
void *fw_mempool_alloc(fw_mempool_t *pool)
{
    struct fw_mempool_s *p = (struct fw_mempool_s *)pool;
    void *user;

    if (p == 0 || p->free_list == 0) { return 0; }

    user = p->free_list;
    /* 弹出链表头, next 指针此前存于 user 首槽 */
    p->free_list = *((void **)user);

    p->free_count--;
    p->used_count++;
    if (p->used_count > p->peak_used) {
        p->peak_used = p->used_count;
    }

#if FW_MEMPOOL_GUARD
    guard_set(user, p->stride);
#endif
    return user;
}

/*============================================================================
 * fw_mempool_free — 校验 ptr 归属后压回空闲链表头
 *============================================================================*/
int fw_mempool_free(fw_mempool_t *pool, void *ptr)
{
    struct fw_mempool_s *p = (struct fw_mempool_s *)pool;
    uint8_t *user_base;
    uint32_t off;

    if (p == 0 || ptr == 0) { return -1; }

    /* 范围 + 对齐校验: ptr 必须落在某个块的 user_ptr 位置 */
    user_base = p->base + FW_MEMPOOL_GUARD_HEAD;
    if ((uint8_t *)ptr < user_base) { return -1; }
    off = (uint32_t)((uint8_t *)ptr - user_base);
    if (p->stride == 0) { return -1; }
    if ((off % p->stride) != 0u) { return -1; }
    if (off >= (p->stride * p->block_count)) { return -1; }

#if FW_MEMPOOL_GUARD
    if (guard_check(ptr, p->stride) != 0) { return -1; }
#endif

    /* 压回链表头: user 首槽写入原链表头 */
    *((void **)ptr) = p->free_list;
    p->free_list = ptr;

    p->free_count++;
    if (p->used_count > 0) { p->used_count--; }
    return 0;
}

/*============================================================================
 * fw_mempool_stats
 *============================================================================*/
uint32_t fw_mempool_stats(fw_mempool_t *pool, uint8_t stat_type)
{
    struct fw_mempool_s *p = (struct fw_mempool_s *)pool;

    if (p == 0) { return 0u; }

    switch (stat_type) {
        case FW_MEMPOOL_STAT_FREE:
            return p->free_count;
        case FW_MEMPOOL_STAT_USED:
            return p->used_count;
        case FW_MEMPOOL_STAT_PEAK:
            return p->peak_used;
        default:
            return 0u;
    }
}
