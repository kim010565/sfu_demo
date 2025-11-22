#pragma once

#include "common.h"

constexpr float FOPI = 1.27323954473516;
constexpr float DP1 = 0.78515625;
constexpr float DP2 = 2.4187564849853515625e-4;
constexpr float DP3 = 3.77489497744594108e-8;
constexpr float lossth = 8192.;
constexpr float T24M1 = 16777215.;

inline vfloat32m4_t sinf(vfloat32m4_t xx, size_t vl) {
  constexpr uint32_t sign_mask = 0x80000000u;
  constexpr float cephes_FOPI = FOPI;
  constexpr float cephes_DP1 = DP1;
  constexpr float cephes_DP2 = DP2;
  constexpr float cephes_DP3 = DP3;
  constexpr float cephes_coscof_p0 = 2.443315711809948E-005;
  constexpr float cephes_coscof_p1 = -1.388731625493765E-003;
  constexpr float cephes_coscof_p2 = 4.166664568298827E-002;
  constexpr float cephes_sincof_p0 = -1.9515295891E-4;
  constexpr float cephes_sincof_p1 = 8.3321608736E-3;
  constexpr float cephes_sincof_p2 = -1.6666654611E-1;
  constexpr float neg_half = -0.5;
  constexpr float one = 1.0;
  constexpr uint32_t one_32 = 1;
  constexpr uint32_t two_32 = 2;
  constexpr uint32_t four_32 = 4;
  constexpr uint32_t inv_one_32 = ~1;
  vfloat32m4_t x = __riscv_vfabs_v_f32m4(xx, vl);  // 取绝对值
  vuint32m4_t sign = __riscv_vand_vx_u32m4(__riscv_vreinterpret_v_f32m4_u32m4(xx), sign_mask, vl);
  // 1）缩小范围
  vfloat32m4_t y = __riscv_vfmul_vf_f32m4(x, cephes_FOPI, vl);
  vuint32m4_t j = __riscv_vfcvt_xu_f_v_u32m4(y, vl);
  j = __riscv_vand_vx_u32m4(__riscv_vadd_vx_u32m4(j, one_32, vl), inv_one_32, vl);
  y = __riscv_vfcvt_f_xu_v_f32m4(j, vl);
  vbool8_t poly_mask = __riscv_vmseq_vx_u32m4_b8(__riscv_vand_vx_u32m4(j, two_32, vl), two_32, vl);
  sign = __riscv_vxor_vv_u32m4(sign, __riscv_vsll_vx_u32m4(__riscv_vand_vx_u32m4(j, four_32, vl), 29, vl), vl);
  x = __riscv_vfnmsub_vf_f32m4(y, cephes_DP1, x, vl);
  x = __riscv_vfnmsub_vf_f32m4(y, cephes_DP2, x, vl);
  x = __riscv_vfnmsub_vf_f32m4(y, cephes_DP3, x, vl);
  // 2）多项式拟合（泰勒级数展开）近似计算sin(x)
  vfloat32m4_t z = __riscv_vfmul_vv_f32m4(x, x, vl);
  vfloat32m4_t y1 = __riscv_vfmadd_vf_f32m4(z, cephes_coscof_p0, __riscv_vfmv_v_f_f32m4(cephes_coscof_p1, vl), vl);
  y1 = __riscv_vfmadd_vv_f32m4(y1, z, __riscv_vfmv_v_f_f32m4(cephes_coscof_p2, vl), vl);
  y1 = __riscv_vfmadd_vv_f32m4(y1, z, __riscv_vfmv_v_f_f32m4(neg_half, vl), vl);
  y1 = __riscv_vfmadd_vv_f32m4(y1, z, __riscv_vfmv_v_f_f32m4(one, vl), vl);
  vfloat32m4_t y2 = __riscv_vfmadd_vf_f32m4(z, cephes_sincof_p0, __riscv_vfmv_v_f_f32m4(cephes_sincof_p1, vl), vl);
  y2 = __riscv_vfmadd_vv_f32m4(y2, z, __riscv_vfmv_v_f_f32m4(cephes_sincof_p2, vl), vl);
  y2 = __riscv_vfmul_vv_f32m4(y2, z, vl);
  y2 = __riscv_vfmadd_vv_f32m4(y2, x, x, vl);
  y = __riscv_vmerge_vvm_f32m4(y2, y1, poly_mask, vl);
  // 3）符号位补偿
  return __riscv_vreinterpret_v_u32m4_f32m4(__riscv_vxor_vv_u32m4(__riscv_vreinterpret_v_f32m4_u32m4(y), sign, vl));
}

inline vfloat32m4_t cosf(vfloat32m4_t xx, size_t vl) {
  constexpr uint32_t sign_mask = 0x80000000u;
  constexpr float cephes_FOPI = FOPI;
  constexpr float cephes_DP1 = DP1;
  constexpr float cephes_DP2 = DP2;
  constexpr float cephes_DP3 = DP3;
  constexpr float cephes_coscof_p0 = 2.443315711809948E-005;
  constexpr float cephes_coscof_p1 = -1.388731625493765E-003;
  constexpr float cephes_coscof_p2 = 4.166664568298827E-002;
  constexpr float cephes_sincof_p0 = -1.9515295891E-4;
  constexpr float cephes_sincof_p1 = 8.3321608736E-3;
  constexpr float cephes_sincof_p2 = -1.6666654611E-1;
  constexpr float neg_half = -0.5;
  constexpr float one = 1.0;
  constexpr uint32_t one_32 = 1;
  constexpr uint32_t two_32 = 2;
  constexpr uint32_t four_32 = 4;
  constexpr uint32_t inv_one_32 = ~1;
  vfloat32m4_t x = __riscv_vfabs_v_f32m4(xx, vl);  // 取绝对值
  // 1）缩小范围
  vfloat32m4_t y = __riscv_vfmul_vf_f32m4(x, cephes_FOPI, vl);
  vuint32m4_t j = __riscv_vfcvt_xu_f_v_u32m4(y, vl);
  j = __riscv_vand_vx_u32m4(__riscv_vadd_vx_u32m4(j, one_32, vl), inv_one_32, vl);
  y = __riscv_vfcvt_f_xu_v_f32m4(j, vl);
  vbool8_t poly_mask = __riscv_vmseq_vx_u32m4_b8(__riscv_vand_vx_u32m4(j, two_32, vl), two_32, vl);
  vuint32m4_t sign = __riscv_vxor_vv_u32m4(__riscv_vsll_vx_u32m4(__riscv_vand_vx_u32m4(j, four_32, vl), 29, vl), __riscv_vsll_vx_u32m4(__riscv_vand_vx_u32m4(j, two_32, vl), 30, vl), vl);
  x = __riscv_vfnmsub_vf_f32m4(y, cephes_DP1, x, vl);
  x = __riscv_vfnmsub_vf_f32m4(y, cephes_DP2, x, vl);
  x = __riscv_vfnmsub_vf_f32m4(y, cephes_DP3, x, vl);
  // 2）多项式拟合（泰勒级数展开）近似计算sin(x)
  vfloat32m4_t z = __riscv_vfmul_vv_f32m4(x, x, vl);
  vfloat32m4_t y1 = __riscv_vfmadd_vf_f32m4(z, cephes_coscof_p0, __riscv_vfmv_v_f_f32m4(cephes_coscof_p1, vl), vl);
  y1 = __riscv_vfmadd_vv_f32m4(y1, z, __riscv_vfmv_v_f_f32m4(cephes_coscof_p2, vl), vl);
  y1 = __riscv_vfmadd_vv_f32m4(y1, z, __riscv_vfmv_v_f_f32m4(neg_half, vl), vl);
  y1 = __riscv_vfmadd_vv_f32m4(y1, z, __riscv_vfmv_v_f_f32m4(one, vl), vl);
  vfloat32m4_t y2 = __riscv_vfmadd_vf_f32m4(z, cephes_sincof_p0, __riscv_vfmv_v_f_f32m4(cephes_sincof_p1, vl), vl);
  y2 = __riscv_vfmadd_vv_f32m4(y2, z, __riscv_vfmv_v_f_f32m4(cephes_sincof_p2, vl), vl);
  y2 = __riscv_vfmul_vv_f32m4(y2, z, vl);
  y2 = __riscv_vfmadd_vv_f32m4(y2, x, x, vl);
  y = __riscv_vmerge_vvm_f32m4(y1, y2, poly_mask, vl);
  // 3）符号位补偿
  return __riscv_vreinterpret_v_u32m4_f32m4(__riscv_vxor_vv_u32m4(__riscv_vreinterpret_v_f32m4_u32m4(y), sign, vl));
}