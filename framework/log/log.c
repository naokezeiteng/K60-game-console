/**
 * @file log.c
 * @brief 零开销分级日志实现
 *
 * 目标6: 零开销分级日志 — 关闭等级在预处理期消除, 启用等级统一走 log_printf
 * 目标2: 自定义段自动注册架构 — log_init 经 FW_AUTO_REGISTER 自动注册
 *
 * 输出格式: "[LEVEL][module:line] message"
 *   - LEVEL   : ERROR/WARN/INFO/DEBUG/TRACE
 *   - module  : FW_LOG_TAG 或 __FILE__ 的 basename
 *   - line    : 调用点 __LINE__
 *
 * 格式化策略:
 *   - 默认 FW_LOG_HAVE_VSPRINTF=1, 使用标准库 vsnprintf (IAR DLIB / GCC 均提供)。
 *   - 置 0 时回退到简易手工拼装 (头部分 + 原样 fmt, 不展开 % 参数),
 *     满足"无 vsnprintf 也可用"的约束。
 */
#include "log.h"
#include "compiler.h"
#include "auto_init.h"
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#ifndef FW_LOG_BUF_SIZE
  #define FW_LOG_BUF_SIZE  128
#endif

/* 默认 1: IAR DLIB 与 GCC 均提供 vsnprintf; 无标准库时置 0 走手工回退 */
#ifndef FW_LOG_HAVE_VSPRINTF
  #define FW_LOG_HAVE_VSPRINTF  1
#endif

/*============================================================================
 * 输出目的地 — 默认空实现, 真机移植时替换为 UART/LCD/RTT 写入
 *============================================================================*/
static void log_default_output(const char *msg)
{
    (void)msg;   /* stub: 真机在此调用 UART 发送 */
}

static void (*log_output)(const char *) = log_default_output;

void log_set_output(fw_log_output_fn fn)
{
    log_output = (fn != 0) ? fn : log_default_output;
}

/*============================================================================
 * 等级 -> 字符串
 *============================================================================*/
static const char *level_str(fw_log_level_t lv)
{
    switch (lv) {
        case LOG_LEVEL_ERROR: return "ERROR";
        case LOG_LEVEL_WARN:  return "WARN";
        case LOG_LEVEL_INFO:  return "INFO";
        case LOG_LEVEL_DEBUG: return "DEBUG";
        case LOG_LEVEL_TRACE: return "TRACE";
        default:              return "NONE";
    }
}

/*============================================================================
 * 路径 basename — 兼容 '/' 与 '\' 分隔符; 无路径时原样返回
 *============================================================================*/
static const char *log_basename(const char *path)
{
    const char *p;
    const char *last;
    if (path == 0) { return "unknown"; }
    p    = path;
    last = path;
    while (*p != '\0') {
        if (*p == '/' || *p == '\\') { last = p + 1; }
        p++;
    }
    return last;
}

/*============================================================================
 * 手工回退用的简易字符串/整数拼接 (仅 FW_LOG_HAVE_VSPRINTF=0 时编译)
 *============================================================================*/
#if !FW_LOG_HAVE_VSPRINTF
static uint32_t log_put_str(char *dst, uint32_t cap, const char *s)
{
    uint32_t i = 0;
    if (cap == 0u) { return 0u; }
    while (s[i] != '\0' && i < (cap - 1u)) {
        dst[i] = s[i];
        i++;
    }
    dst[i] = '\0';
    return i;
}

static uint32_t log_put_uint(char *dst, uint32_t cap, unsigned int v)
{
    char tmp[10];
    uint32_t len = 0;
    uint32_t i;
    do {
        tmp[len] = (char)('0' + (v % 10u));
        len++;
        v /= 10u;
    } while (v != 0u && len < (uint32_t)sizeof(tmp));
    if (cap == 0u) { return 0u; }
    for (i = 0; i < len && (i + 1u) < cap; i++) {
        dst[i] = tmp[len - 1u - i];
    }
    dst[i] = '\0';
    return i;
}
#endif /* !FW_LOG_HAVE_VSPRINTF */

/*============================================================================
 * log_printf — 统一格式化入口
 *============================================================================*/
void log_printf(fw_log_level_t level, const char *file, int line,
                const char *fmt, ...)
{
    char buf[FW_LOG_BUF_SIZE];
    int n;
    const char *mod;
    va_list ap;

    /* 运行期兜底过滤 (编译期已消除关闭等级的调用) */
    if ((int)level > (int)LOG_LEVEL) { return; }
    if (log_output == 0) { return; }

    mod = log_basename(file);

#if FW_LOG_HAVE_VSPRINTF
    n = snprintf(buf, FW_LOG_BUF_SIZE, "[%s][%s:%d] ",
                 level_str(level), mod, line);
    if (n < 0) { n = 0; }
    if (n >= FW_LOG_BUF_SIZE) { n = FW_LOG_BUF_SIZE - 1; }

    va_start(ap, fmt);
    {
        int m = vsnprintf(&buf[n], (FW_LOG_BUF_SIZE - n), fmt, ap);
        va_end(ap);
        if (m < 0) { m = 0; }
        n += m;
    }
    if (n >= FW_LOG_BUF_SIZE) { n = FW_LOG_BUF_SIZE - 1; }
    buf[n] = '\0';
#else
    /* 手工回退: 拼装头部后原样追加 fmt (不展开 % 参数) */
    {
        int m = 0;
        n = 0;
        m  = (int)log_put_str(&buf[n], (uint32_t)(FW_LOG_BUF_SIZE - n), "[");
        n += m;
        m  = (int)log_put_str(&buf[n], (uint32_t)(FW_LOG_BUF_SIZE - n), level_str(level));
        n += m;
        m  = (int)log_put_str(&buf[n], (uint32_t)(FW_LOG_BUF_SIZE - n), "][");
        n += m;
        m  = (int)log_put_str(&buf[n], (uint32_t)(FW_LOG_BUF_SIZE - n), mod);
        n += m;
        m  = (int)log_put_str(&buf[n], (uint32_t)(FW_LOG_BUF_SIZE - n), ":");
        n += m;
        m  = (int)log_put_uint(&buf[n], (uint32_t)(FW_LOG_BUF_SIZE - n), (unsigned int)line);
        n += m;
        m  = (int)log_put_str(&buf[n], (uint32_t)(FW_LOG_BUF_SIZE - n), "] ");
        n += m;
        m  = (int)log_put_str(&buf[n], (uint32_t)(FW_LOG_BUF_SIZE - n), fmt ? fmt : "(null)");
        n += m;
    }
    if (n >= FW_LOG_BUF_SIZE) { n = FW_LOG_BUF_SIZE - 1; }
    buf[n] = '\0';
    (void)ap;     /* 回退分支不用 va_list, 消除未使用告警 */
#endif

    log_output(buf);
}

/*============================================================================
 * log_hex_dump — 十六进制转储, 仅 DEBUG 及以上编译; 不依赖 vsnprintf
 *============================================================================*/
#if (LOG_LEVEL >= FW_LOG_LV_DEBUG)
void log_hex_dump(const void *data, uint32_t len)
{
    static const char hex_chars[] = "0123456789ABCDEF";
    char buf[FW_LOG_BUF_SIZE];
    const uint8_t *p = (const uint8_t *)data;
    uint32_t i;
    int k;

    if (data == 0 || log_output == 0) { return; }

    k = 0;
    for (i = 0; i < len; i++) {
        if ((k + 4) >= FW_LOG_BUF_SIZE) {
            buf[k] = '\0';
            log_output(buf);
            k = 0;
        }
        buf[k++] = hex_chars[(p[i] >> 4) & 0x0F];
        buf[k++] = hex_chars[p[i] & 0x0F];
        buf[k++] = ' ';
        if ((i & 0x0Fu) == 0x0Fu) {
            buf[k] = '\0';
            log_output(buf);
            k = 0;
        }
    }
    if (k > 0) {
        buf[k] = '\0';
        log_output(buf);
    }
}
#endif /* LOG_LEVEL >= FW_LOG_LV_DEBUG */

/*============================================================================
 * 初始化 — 恢复默认输出, 自动注册到 SERVICE 优先级
 *============================================================================*/
void log_init(void)
{
    log_output = log_default_output;
}

FW_AUTO_REGISTER(log, log_init, FW_PRIO_SERVICE);
