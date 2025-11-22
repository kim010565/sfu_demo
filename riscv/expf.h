#pragma once

#include "common.h"

constexpr float C1 = 0.693359375;
constexpr float C2 = -2.12194440e-4;

inline vfloat32m4_t expf(vfloat32m4_t xx, size_t vl) {
  constexpr float exp_hi = MAXLOGF;
  constexpr float exp_lo = -MAXLOGF;
  constexpr float cephes_LOG2EF = LOG2EF;
  constexpr float cephes_C1 = C1;
  constexpr float cephes_C2 = C2;
  constexpr float cephes_exp_p0 = 1.9875691500E-4;
  constexpr float cephes_exp_p1 = 1.3981999507E-3;
  constexpr float cephes_exp_p2 = 8.3334519073E-3;
  constexpr float cephes_exp_p3 = 4.1665795894E-2;
  constexpr float cephes_exp_p4 = 1.6666665459E-1;
  constexpr float cephes_exp_p5 = 5.0000001201E-1;
  constexpr float one = 1.0;
  constexpr float half = 0.5;
  // 上限、下限保护
  vfloat32m4_t x = __riscv_vfmax_vf_f32m4(__riscv_vfmin_vf_f32m4(xx, exp_hi, vl), exp_lo, vl);
  // 1) 缩小范围
  vfloat32m4_t z = __riscv_vfcvt_f_x_v_f32m4(__riscv_vfcvt_x_f_v_i32m4_rm(__riscv_vfmadd_vf_f32m4(x, cephes_LOG2EF, __riscv_vfmv_v_f_f32m4(half, vl), vl), 2, vl), vl);
  x = __riscv_vfnmsub_vf_f32m4(z, cephes_C1, x, vl);
  x = __riscv_vfnmsub_vf_f32m4(z, cephes_C2, x, vl);
  vfloat32m4_t n = z;
  // 2）多项式拟合（泰勒级数展开）近似计算e^g
  z = __riscv_vfmul_vv_f32m4(x, x, vl);
  vfloat32m4_t y = __riscv_vfmadd_vf_f32m4(x, cephes_exp_p0, __riscv_vfmv_v_f_f32m4(cephes_exp_p1, vl), vl);
  vfloat32m4_t y1 = __riscv_vfmadd_vf_f32m4(x, cephes_exp_p3, __riscv_vfmv_v_f_f32m4(cephes_exp_p4, vl), vl);
  vfloat32m4_t y2 = __riscv_vfadd_vf_f32m4(x, one, vl);
  y = __riscv_vfmadd_vv_f32m4(y, x, __riscv_vfmv_v_f_f32m4(cephes_exp_p2, vl), vl);
  y1 = __riscv_vfmadd_vv_f32m4(y1, x, __riscv_vfmv_v_f_f32m4(cephes_exp_p5, vl), vl);
  y = __riscv_vfmadd_vv_f32m4(y, __riscv_vfmul_vv_f32m4(z, x, vl), y1, vl);
  y = __riscv_vfmadd_vv_f32m4(y, z, y2, vl);
  // 3）恢复范围，计算e^g * 2^n
  return __riscv_vfmax_vv_f32m4(ldexp(y, n, vl), xx, vl);
}