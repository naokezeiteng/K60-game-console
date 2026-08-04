/**
 * @file compiler.h
 * @brief 编译器抽象层 — IAR/GCC跨平台属性、静态断言、编译期常量固化
 *
 * 目标1: 编译期全量优化校验 — 静态断言前置校验
 * 目标7: 编译期强类型风控 — 编译器属性拦截
 */
#ifndef FRAMEWORK_CORE_COMPILER_H
#define FRAMEWORK_CORE_COMPILER_H

/*============================================================================
 * 编译器检测
 *============================================================================*/
#if defined(__ICCARM__)
  #define COMPILER_IAR    1
  #define COMPILER_GCC    0
#elif defined(__GNUC__)
  #define COMPILER_IAR    0
  #define COMPILER_GCC    1
#else
  #error "Unsupported compiler. Only IAR and GCC are supported."
#endif

/*============================================================================
 * 自定义段声明 — 用于自动注册架构
 * IAR: #pragma section / __root
 * GCC: __attribute__((section, used))
 *============================================================================*/
#if COMPILER_IAR
  /* IAR: 声明一个自定义只读段 */
  #define FW_SECTION_ROM(name) \
      _Pragma(#name)

  /* IAR: 将变量放入指定段 */
  #define FW_PLACE_ROM(section_name) \
      @ section_name

  /* IAR: 防止死代码消除 */
  #define FW_ROOT  __root

  /* IAR: 获取段起始/结束地址 */
  #define FW_SECTION_BEGIN(section_name)  __section_begin(section_name)
  #define FW_SECTION_END(section_name)    __section_end(section_name)
  #define FW_SECTION_SIZE(section_name)   __section_size(section_name)

#elif COMPILER_GCC
  #define FW_SECTION_ROM(name)

  #define FW_PLACE_ROM(section_name) \
      __attribute__((section(#section_name)))

  #define FW_ROOT  __attribute__((used))

  /* GCC: 使用 linker symbol 获取段边界 */
  #define FW_SECTION_BEGIN(section_name)  (&_s_##section_name)
  #define FW_SECTION_END(section_name)    (&_e_##section_name)

#endif

/*============================================================================
 * 静态断言 — 编译期常量校验，零运行时开销
 * C11 _Static_assert 优先，回退到 typedef 技巧
 *============================================================================*/
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
  #define FW_STATIC_ASSERT(cond, msg)  _Static_assert(cond, msg)
#else
  /* 经典 typedef 技巧：cond 为假时数组大小为负，编译报错 */
  #define FW_STATIC_ASSERT(cond, msg) \
      typedef char __FW_ASSERT_##msg[(cond) ? 1 : -1]
#endif

/* 简化版：仅条件，无自定义消息 */
#define FW_STATIC_ASSERT_X(cond)  FW_STATIC_ASSERT(cond, static_assert_failed)

/*============================================================================
 * 编译期常量固化 — 将可计算逻辑前置至编译阶段
 *============================================================================*/

/* 编译期 array_size — 替代 sizeof(arr)/sizeof(arr[0])，拒绝指针 */
#define FW_ARRAY_SIZE(arr) \
    (sizeof(arr) / sizeof((arr)[0]) + \
     FW_STATIC_ASSERT_EXPR(FW_IS_ARRAY(arr), arr_must_be_array))

/* 判断是否为数组（非指针） */
#define FW_IS_ARRAY(arr) \
    (!__builtin_types_compatible_p(typeof(arr), typeof(&(arr)[0])))

/* 静态断言表达式版本 — 可用于初始化列表 */
#define FW_STATIC_ASSERT_EXPR(cond, msg) \
    (sizeof(struct { int msg : (cond) ? 1 : -1; }))

/* 编译期 min/max */
#define FW_MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define FW_MAX(a, b)  (((a) > (b)) ? (a) : (b))

/* 编译期位运算工具 */
#define FW_BIT(n)           (1U << (n))
#define FW_MASK(hi, lo)     ((FW_BIT((hi) + 1) - 1) & ~(FW_BIT(lo) - 1))
#define FW_ALIGN_UP(x, a)   (((x) + (a) - 1) & ~((a) - 1))
#define FW_ALIGN_DOWN(x, a) ((x) & ~((a) - 1))

/*============================================================================
 * 强类型枚举 — 编译期类型隔离，防止枚举混用
 *============================================================================*/
/**
 * FW_STRONG_ENUM(name, type, list) 生成强类型枚举 + 字符串映射表
 * 用法:
 *   FW_STRONG_ENUM(color, uint8_t,
 *       (COLOR_RED,   0)
 *       (COLOR_GREEN, 1)
 *       (COLOR_BLUE,  2)
 *   )
 * 生成:
 *   typedef uint8_t color_t;
 *   enum { COLOR_RED = 0, COLOR_GREEN = 1, COLOR_BLUE = 2 };
 *   static const char* color_str[] = { "COLOR_RED", "COLOR_GREEN", "COLOR_BLUE" };
 */
#define FW_STRONG_ENUM(name, type, ...) \
    typedef type name##_t; \
    FW_STRONG_ENUM_##name \
    FW_STRONG_ENUM_IMPL(name, __VA_ARGS__)

/*============================================================================
 * 编译期字符串映射 — 枚举值↔字符串自动关联
 *============================================================================*/

/* X-Macro: 枚举字符串自动映射 */
#define FW_ENUM_TO_STR(val, ...)  #val,

/*============================================================================
 * likely/unlikely — 分支预测优化
 *============================================================================*/
#if COMPILER_GCC
  #define FW_LIKELY(x)   __builtin_expect(!!(x), 1)
  #define FW_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
  #define FW_LIKELY(x)   (x)
  #define FW_UNLIKELY(x) (x)
#endif

/*============================================================================
 * 对齐属性
 *============================================================================*/
#if COMPILER_GCC
  #define FW_ALIGNED(n)  __attribute__((aligned(n)))
  #define FW_PACKED      __attribute__((packed))
#elif COMPILER_IAR
  #define FW_ALIGNED(n)  _Pragma(#n)
  #define FW_PACKED      __packed
#endif

/*============================================================================
 * 不透明类型 — 隐藏模块内部结构，消除跨文件耦合
 *============================================================================*/
/** 前向声明不透明结构体，头文件仅暴露指针 */
#define FW_OPAQUE_TYPE(name) \
    typedef struct name##_s name##_t

/** 定义不透明结构体，仅在 .c 文件中使用 */
#define FW_OPAQUE_IMPL(name) \
    struct name##_s

/*============================================================================
 * 编译期拦截 — 禁止拷贝/禁止栈分配
 *============================================================================*/
#if COMPILER_GCC
  #define FW_NO_COPY(type) \
      type(const type&) = delete; \
      type& operator=(const type&) = delete
#else
  /* IAR C模式无delete，用私有化声明替代 */
  #define FW_NO_COPY(type) \
      type(const type&); \
      type& operator=(const type&)
#endif

/*============================================================================
 * pure/const 函数属性 — 帮助编译器优化
 *============================================================================*/
#if COMPILER_GCC
  #define FW_PURE   __attribute__((pure))
  #define FW_CONST  __attribute__((const))
#else
  #define FW_PURE
  #define FW_CONST
#endif

/*============================================================================
 * 标记变量为可在中断中修改
 *============================================================================*/
#define FW_VOLATILE  volatile

/*============================================================================
 * 模块配置: 编译模式
 *============================================================================*/
#ifndef FW_DEBUG_MODE
  #define FW_DEBUG_MODE  0
#endif

#ifndef FW_RELEASE_MODE
  #define FW_RELEASE_MODE  1
#endif

#endif /* FRAMEWORK_CORE_COMPILER_H */
