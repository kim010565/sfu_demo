#pragma once

#include "common.h"

inline vfloat32m4_t logf(vfloat32m4_t xx, size_t vl) {
  constexpr uint32_t min_norm_pos = 0x00800000u;
  constexpr uint32_t minus_inf = 0xff800000u;
  constexpr uint32_t pos_inf = 0x7f800000u;
  constexpr float cephes_SQRTHF = SQRTHF;
  constexpr float cephes_log_p0 = 7.0376836292E-2;
  constexpr float cephes_log_p1 = -1.1514610310E-1;
  constexpr float cephes_log_p2 = 1.1676998740E-1;
  constexpr float cephes_log_p3 = -1.2420140846E-1;
  constexpr float cephes_log_p4 = 1.4249322787E-1;
  constexpr float cephes_log_p5 = -1.6668057665E-1;
  constexpr float cephes_log_p6 = 2.0000714765E-1;
  constexpr float cephes_log_p7 = -2.4999993993E-1;
  constexpr float cephes_log_p8 = 3.3333331174E-1;
  constexpr float zero = 0.0;
  constexpr float one = 1.0;
  constexpr float neg_half = -0.5;
  constexpr float cephes_log2 = 0.69314718055994530941723212145818;
  // 输入范围保护
  vfloat32m4_t x = __riscv_vfmax_vf_f32m4(xx, *(float*)(&min_norm_pos), vl);
  // 1) 缩小范围
  vfloat32m4_t e;
  x = frexpf(x, e, vl);
  vbool8_t mask = __riscv_vmflt_vf_f32m4_b8(x, cephes_SQRTHF, vl);
  x = __riscv_vfadd_vv_f32m4_m(mask, x, x, vl);
  x = __riscv_vfsub_vf_f32m4(x, one, vl);
  e = __riscv_vfsub_vf_f32m4_m(mask, e, one, vl);
  // 多项式拟合（泰勒级数展开）近似计算log(1+r)
  vfloat32m4_t z = __riscv_vfmul_vv_f32m4(x, x, vl);
  vfloat32m4_t x3 = __riscv_vfmul_vv_f32m4(x, z, vl);
  vfloat32m4_t y = __riscv_vfmadd_vf_f32m4(x, cephes_log_p0, __riscv_vfmv_v_f_f32m4(cephes_log_p1, vl), vl);
  vfloat32m4_t y1 = __riscv_vfmadd_vf_f32m4(x, cephes_log_p3, __riscv_vfmv_v_f_f32m4(cephes_log_p4, vl), vl);
  vfloat32m4_t y2 = __riscv_vfmadd_vf_f32m4(x, cephes_log_p6, __riscv_vfmv_v_f_f32m4(cephes_log_p7, vl), vl);
  y = __riscv_vfmadd_vv_f32m4(y, x, __riscv_vfmv_v_f_f32m4(cephes_log_p2, vl), vl);
  y1 = __riscv_vfmadd_vv_f32m4(y1, x, __riscv_vfmv_v_f_f32m4(cephes_log_p5, vl), vl);
  y2 = __riscv_vfmadd_vv_f32m4(y2, x, __riscv_vfmv_v_f_f32m4(cephes_log_p8, vl), vl);
  y = __riscv_vfmadd_vv_f32m4(y, x3, y1, vl);
  y = __riscv_vfmadd_vv_f32m4(y, x3, y2, vl);
  y = __riscv_vfmul_vv_f32m4(y, x3, vl);
  y = __riscv_vfmadd_vf_f32m4(z, neg_half, y, vl);
  z = __riscv_vfadd_vv_f32m4(x, y, vl);
  // 3）恢复范围，计算e*log(2) + log(1+r)
  z = __riscv_vfmadd_vf_f32m4(e, cephes_log2, z, vl);
  vbool8_t invalid_mask = __riscv_vmflt_vf_f32m4_b8(xx, zero, vl);
  vbool8_t iszero_mask = __riscv_vmfeq_vf_f32m4_b8(xx, zero, vl);
  vbool8_t pos_inf_mask = __riscv_vmfeq_vf_f32m4_b8(xx, *(float*)(&pos_inf), vl);
  return __riscv_vfmerge_vfm_f32m4(__riscv_vreinterpret_v_u32m4_f32m4(__riscv_vor_vx_u32m4_m(invalid_mask, __riscv_vreinterpret_v_f32m4_u32m4(__riscv_vfmerge_vfm_f32m4(z, *(float*)(&pos_inf), pos_inf_mask, vl)), 0xFFFFFFFF, vl)), *(float*)(&minus_inf), iszero_mask, vl);
}