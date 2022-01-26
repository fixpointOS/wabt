/* Automically generated by wasm2c */
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "fac.h"
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#define LIKELY(x) __builtin_expect(!!(x), 1)

#define TRAP(x) (wasm_rt_trap(WASM_RT_TRAP_##x), 0)

#define FUNC_PROLOGUE                                            \
  if (++module_instance->wasm_rt_call_stack_depth > WASM_RT_MAX_CALL_STACK_DEPTH) \
    TRAP(EXHAUSTION)

#define FUNC_EPILOGUE --module_instance->wasm_rt_call_stack_depth

#define UNREACHABLE TRAP(UNREACHABLE)

#define CALL_INDIRECT(table, t, ft, x, ...)          \
  (LIKELY((x) < table.size && table.data[x].func &&  \
          table.data[x].func_type == func_types[ft]) \
       || TRAP(CALL_INDIRECT)                        \
       , ((t)table.data[x].func)(__VA_ARGS__))

#define RANGE_CHECK(mem, a, t) \
  if (UNLIKELY((a) + sizeof(t) > mem->size)) TRAP(OOB)

#if WASM_RT_MEMCHECK_SIGNAL_HANDLER
#define MEMCHECK(mem, a, t)
#else
#define MEMCHECK(mem, a, t) RANGE_CHECK(mem, a, t)
#endif

#if WABT_BIG_ENDIAN
static inline void load_data(void *dest, const void *src, size_t n) {
  size_t i = 0;
  u8 *dest_chars = dest;
  memcpy(dest, src, n);
  for (i = 0; i < (n>>1); i++) {
    u8 cursor = dest_chars[i];
    dest_chars[i] = dest_chars[n - i - 1];
    dest_chars[n - i - 1] = cursor;
  }
}
#define LOAD_DATA(m, o, i, s) do {              \
    RANGE_CHECK((&m), m.size - o - s, char[s]); \
    load_data(&(m.data[m.size - o - s]), i, s); \
  } while (0)
#define DEFINE_LOAD(name, t1, t2, t3)                                                 \
  static inline t3 name(wasm_rt_memory_t* mem, u64 addr) {                            \
    MEMCHECK(mem, addr, t1);                                                          \
    t1 result;                                                                        \
    __builtin_memcpy(&result, &mem->data[mem->size - addr - sizeof(t1)], sizeof(t1)); \
    return (t3)(t2)result;                                                            \
  }

#define DEFINE_STORE(name, t1, t2)                                                     \
  static inline void name(wasm_rt_memory_t* mem, u64 addr, t2 value) {                 \
    MEMCHECK(mem, addr, t1);                                                           \
    t1 wrapped = (t1)value;                                                            \
    __builtin_memcpy(&mem->data[mem->size - addr - sizeof(t1)], &wrapped, sizeof(t1)); \
  }
#else
static inline void load_data(void *dest, const void *src, size_t n) {
  memcpy(dest, src, n);
}
#define LOAD_DATA(m, o, i, s) do { \
    RANGE_CHECK((&m), o, char[s]); \
    load_data(&(m.data[o]), i, s); \
  } while (0)
#define DEFINE_LOAD(name, t1, t2, t3)                        \
  static inline t3 name(wasm_rt_memory_t* mem, u64 addr) {   \
    MEMCHECK(mem, addr, t1);                                 \
    t1 result;                                               \
    __builtin_memcpy(&result, &mem->data[addr], sizeof(t1)); \
    return (t3)(t2)result;                                   \
  }

#define DEFINE_STORE(name, t1, t2)                                     \
  static inline void name(wasm_rt_memory_t* mem, u64 addr, t2 value) { \
    MEMCHECK(mem, addr, t1);                                           \
    t1 wrapped = (t1)value;                                            \
    __builtin_memcpy(&mem->data[addr], &wrapped, sizeof(t1));          \
  }
#endif

DEFINE_LOAD(i32_load, u32, u32, u32)
DEFINE_LOAD(i64_load, u64, u64, u64)
DEFINE_LOAD(f32_load, f32, f32, f32)
DEFINE_LOAD(f64_load, f64, f64, f64)
DEFINE_LOAD(i32_load8_s, s8, s32, u32)
DEFINE_LOAD(i64_load8_s, s8, s64, u64)
DEFINE_LOAD(i32_load8_u, u8, u32, u32)
DEFINE_LOAD(i64_load8_u, u8, u64, u64)
DEFINE_LOAD(i32_load16_s, s16, s32, u32)
DEFINE_LOAD(i64_load16_s, s16, s64, u64)
DEFINE_LOAD(i32_load16_u, u16, u32, u32)
DEFINE_LOAD(i64_load16_u, u16, u64, u64)
DEFINE_LOAD(i64_load32_s, s32, s64, u64)
DEFINE_LOAD(i64_load32_u, u32, u64, u64)
DEFINE_STORE(i32_store, u32, u32)
DEFINE_STORE(i64_store, u64, u64)
DEFINE_STORE(f32_store, f32, f32)
DEFINE_STORE(f64_store, f64, f64)
DEFINE_STORE(i32_store8, u8, u32)
DEFINE_STORE(i32_store16, u16, u32)
DEFINE_STORE(i64_store8, u8, u64)
DEFINE_STORE(i64_store16, u16, u64)
DEFINE_STORE(i64_store32, u32, u64)

#define I32_CLZ(x) ((x) ? __builtin_clz(x) : 32)
#define I64_CLZ(x) ((x) ? __builtin_clzll(x) : 64)
#define I32_CTZ(x) ((x) ? __builtin_ctz(x) : 32)
#define I64_CTZ(x) ((x) ? __builtin_ctzll(x) : 64)
#define I32_POPCNT(x) (__builtin_popcount(x))
#define I64_POPCNT(x) (__builtin_popcountll(x))

#define DIV_S(ut, min, x, y)                                 \
   ((UNLIKELY((y) == 0)) ?                TRAP(DIV_BY_ZERO)  \
  : (UNLIKELY((x) == min && (y) == -1)) ? TRAP(INT_OVERFLOW) \
  : (ut)((x) / (y)))

#define REM_S(ut, min, x, y)                                \
   ((UNLIKELY((y) == 0)) ?                TRAP(DIV_BY_ZERO) \
  : (UNLIKELY((x) == min && (y) == -1)) ? 0                 \
  : (ut)((x) % (y)))

#define I32_DIV_S(x, y) DIV_S(u32, INT32_MIN, (s32)x, (s32)y)
#define I64_DIV_S(x, y) DIV_S(u64, INT64_MIN, (s64)x, (s64)y)
#define I32_REM_S(x, y) REM_S(u32, INT32_MIN, (s32)x, (s32)y)
#define I64_REM_S(x, y) REM_S(u64, INT64_MIN, (s64)x, (s64)y)

#define DIVREM_U(op, x, y) \
  ((UNLIKELY((y) == 0)) ? TRAP(DIV_BY_ZERO) : ((x) op (y)))

#define DIV_U(x, y) DIVREM_U(/, x, y)
#define REM_U(x, y) DIVREM_U(%, x, y)

#define ROTL(x, y, mask) \
  (((x) << ((y) & (mask))) | ((x) >> (((mask) - (y) + 1) & (mask))))
#define ROTR(x, y, mask) \
  (((x) >> ((y) & (mask))) | ((x) << (((mask) - (y) + 1) & (mask))))

#define I32_ROTL(x, y) ROTL(x, y, 31)
#define I64_ROTL(x, y) ROTL(x, y, 63)
#define I32_ROTR(x, y) ROTR(x, y, 31)
#define I64_ROTR(x, y) ROTR(x, y, 63)

#define FMIN(x, y)                                          \
   ((UNLIKELY((x) != (x))) ? NAN                            \
  : (UNLIKELY((y) != (y))) ? NAN                            \
  : (UNLIKELY((x) == 0 && (y) == 0)) ? (signbit(x) ? x : y) \
  : (x < y) ? x : y)

#define FMAX(x, y)                                          \
   ((UNLIKELY((x) != (x))) ? NAN                            \
  : (UNLIKELY((y) != (y))) ? NAN                            \
  : (UNLIKELY((x) == 0 && (y) == 0)) ? (signbit(x) ? y : x) \
  : (x > y) ? x : y)

#define TRUNC_S(ut, st, ft, min, minop, max, x)                             \
  ((UNLIKELY((x) != (x)))                        ? TRAP(INVALID_CONVERSION) \
   : (UNLIKELY(!((x)minop(min) && (x) < (max)))) ? TRAP(INT_OVERFLOW)       \
                                                 : (ut)(st)(x))

#define I32_TRUNC_S_F32(x) TRUNC_S(u32, s32, f32, (f32)INT32_MIN, >=, 2147483648.f, x)
#define I64_TRUNC_S_F32(x) TRUNC_S(u64, s64, f32, (f32)INT64_MIN, >=, (f32)INT64_MAX, x)
#define I32_TRUNC_S_F64(x) TRUNC_S(u32, s32, f64, -2147483649., >, 2147483648., x)
#define I64_TRUNC_S_F64(x) TRUNC_S(u64, s64, f64, (f64)INT64_MIN, >=, (f64)INT64_MAX, x)

#define TRUNC_U(ut, ft, max, x)                                            \
  ((UNLIKELY((x) != (x)))                       ? TRAP(INVALID_CONVERSION) \
   : (UNLIKELY(!((x) > (ft)-1 && (x) < (max)))) ? TRAP(INT_OVERFLOW)       \
                                                : (ut)(x))

#define I32_TRUNC_U_F32(x) TRUNC_U(u32, f32, 4294967296.f, x)
#define I64_TRUNC_U_F32(x) TRUNC_U(u64, f32, (f32)UINT64_MAX, x)
#define I32_TRUNC_U_F64(x) TRUNC_U(u32, f64, 4294967296.,  x)
#define I64_TRUNC_U_F64(x) TRUNC_U(u64, f64, (f64)UINT64_MAX, x)

#define TRUNC_SAT_S(ut, st, ft, min, smin, minop, max, smax, x) \
  ((UNLIKELY((x) != (x)))         ? 0                           \
   : (UNLIKELY(!((x)minop(min)))) ? smin                        \
   : (UNLIKELY(!((x) < (max))))   ? smax                        \
                                  : (ut)(st)(x))

#define I32_TRUNC_SAT_S_F32(x) TRUNC_SAT_S(u32, s32, f32, (f32)INT32_MIN, INT32_MIN, >=, 2147483648.f, INT32_MAX, x)
#define I64_TRUNC_SAT_S_F32(x) TRUNC_SAT_S(u64, s64, f32, (f32)INT64_MIN, INT64_MIN, >=, (f32)INT64_MAX, INT64_MAX, x)
#define I32_TRUNC_SAT_S_F64(x) TRUNC_SAT_S(u32, s32, f64, -2147483649., INT32_MIN, >, 2147483648., INT32_MAX, x)
#define I64_TRUNC_SAT_S_F64(x) TRUNC_SAT_S(u64, s64, f64, (f64)INT64_MIN, INT64_MIN, >=, (f64)INT64_MAX, INT64_MAX, x)

#define TRUNC_SAT_U(ut, ft, max, smax, x) \
  ((UNLIKELY((x) != (x)))        ? 0      \
   : (UNLIKELY(!((x) > (ft)-1))) ? 0      \
   : (UNLIKELY(!((x) < (max))))  ? smax   \
                                 : (ut)(x))

#define I32_TRUNC_SAT_U_F32(x) TRUNC_SAT_U(u32, f32, 4294967296.f, UINT32_MAX, x)
#define I64_TRUNC_SAT_U_F32(x) TRUNC_SAT_U(u64, f32, (f32)UINT64_MAX, UINT64_MAX, x)
#define I32_TRUNC_SAT_U_F64(x) TRUNC_SAT_U(u32, f64, 4294967296., UINT32_MAX,  x)
#define I64_TRUNC_SAT_U_F64(x) TRUNC_SAT_U(u64, f64, (f64)UINT64_MAX, UINT64_MAX, x)

#define DEFINE_REINTERPRET(name, t1, t2)  \
  static inline t2 name(t1 x) {           \
    t2 result;                            \
    memcpy(&result, &x, sizeof(result));  \
    return result;                        \
  }

DEFINE_REINTERPRET(f32_reinterpret_i32, u32, f32)
DEFINE_REINTERPRET(i32_reinterpret_f32, f32, u32)
DEFINE_REINTERPRET(f64_reinterpret_i64, u64, f64)
DEFINE_REINTERPRET(i64_reinterpret_f64, f64, u64)

static u32 func_types[1];
static void init_func_types(void) {
  func_types[0] = wasm_rt_register_func_type(1, 1, WASM_RT_I32, WASM_RT_I32);
}

static u32 w2c_fac(Z_fac_module_instance_t *, u32);

static u32 w2c_fac(Z_fac_module_instance_t *module_instance, u32 w2c_p0) {
  FUNC_PROLOGUE;
  u32 w2c_i0, w2c_i1, w2c_i2;
  w2c_i0 = w2c_p0;
  w2c_i1 = 0u;
  w2c_i0 = w2c_i0 == w2c_i1;
  if (w2c_i0) {
    w2c_i0 = 1u;
  } else {
    w2c_i0 = w2c_p0;
    w2c_i1 = w2c_p0;
    w2c_i2 = 1u;
    w2c_i1 -= w2c_i2;
    w2c_i1 = w2c_fac(module_instance, w2c_i1);
    w2c_i0 *= w2c_i1;
  }
  FUNC_EPILOGUE;
  return w2c_i0;
}

static void init_globals(Z_fac_module_instance_t *module_instance) {
}


static void init_memory(Z_fac_module_instance_t *module_instance) {
  wasm_rt_allocate_memory(&module_instance->w2c_M0, 1, 65536);
}

static void init_table(Z_fac_module_instance_t *module_instance) {
  uint32_t offset;
}
/* export: 'fac' */
u32 Z_facZ_fac(Z_fac_module_instance_t *module_instance, u32 w2c_p0) {
  return w2c_fac(module_instance, w2c_p0);
}


void Z_fac_init_module(){
  init_func_types();
}

void Z_fac_init(Z_fac_module_instance_t* module_instance) {
  init_globals(module_instance);
  init_memory(module_instance);
  init_table(module_instance);
  module_instance->wasm_rt_call_stack_depth = 0;
}

void Z_fac_free(Z_fac_module_instance_t* module_instance) {
}
