#pragma once

#include "common.h"

constexpr float FOPI = 1.27323954473516;
constexpr float DP1 = 0.78515625;
constexpr float DP2 = 2.4187564849853515625e-4;
constexpr float DP3 = 3.77489497744594108e-8;
constexpr float lossth = 8192.;
constexpr float T24M1 = 16777215.;

inline float32x4_t sinf(float32x4_t xx) {
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
  float32x4_t x = vabsq_f32(xx);  // 取绝对值
  uint32x4_t sign = vandq_u32(vreinterpretq_u32_f32(xx), vdupq_n_u32(sign_mask));
  // 1）缩小范围
  float32x4_t y = vmulq_n_f32(x, cephes_FOPI);
  uint32x4_t j = vcvtq_u32_f32(y);
  j = vandq_u32(vaddq_u32(j, vdupq_n_u32(one_32)), vdupq_n_u32(inv_one_32));
  y = vcvtq_f32_u32(j);
  uint32x4_t poly_mask = vceqq_u32(vandq_u32(j, vdupq_n_u32(two_32)), vdupq_n_u32(two_32));
  sign = veorq_u32(sign, vshlq_n_u32(vandq_u32(j, vdupq_n_u32(four_32)), 29));
  x = vmlsq_n_f32(x, y, cephes_DP1);
  x = vmlsq_n_f32(x, y, cephes_DP2);
  x = vmlsq_n_f32(x, y, cephes_DP3);
  // 2）多项式拟合（泰勒级数展开）近似计算sin(x)
  float32x4_t z = vmulq_f32(x, x);
  float32x4_t y1 = vmlaq_n_f32(vdupq_n_f32(cephes_coscof_p1), z, cephes_coscof_p0);
  y1 = vmlaq_f32(vdupq_n_f32(cephes_coscof_p2), y1, z);
  y1 = vmlaq_f32(vdupq_n_f32(neg_half), y1, z);
  y1 = vmlaq_f32(vdupq_n_f32(one), y1, z);
  float32x4_t y2 = vmlaq_n_f32(vdupq_n_f32(cephes_sincof_p1), z, cephes_sincof_p0);
  y2 = vmlaq_f32(vdupq_n_f32(cephes_sincof_p2), y2, z);
  y2 = vmulq_f32(y2, z);
  y2 = vmlaq_f32(x, y2, x);
  y = vbslq_f32(poly_mask, y1, y2);
  // 3）符号位补偿
  return vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(y), sign));
}

inline float32x4_t cosf(float32x4_t xx) {
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
  float32x4_t x = vabsq_f32(xx);  // 取绝对值
  // 1）缩小范围
  float32x4_t y = vmulq_n_f32(x, cephes_FOPI);
  uint32x4_t j = vcvtq_u32_f32(y);
  j = vandq_u32(vaddq_u32(j, vdupq_n_u32(one_32)), vdupq_n_u32(inv_one_32));
  y = vcvtq_f32_u32(j);
  uint32x4_t poly_mask = vceqq_u32(vandq_u32(j, vdupq_n_u32(two_32)), vdupq_n_u32(two_32));
  uint32x4_t sign = veorq_u32(vshlq_n_u32(vandq_u32(j, vdupq_n_u32(four_32)), 29), vshlq_n_u32(vandq_u32(j, vdupq_n_u32(two_32)), 30));
  x = vmlsq_n_f32(x, y, cephes_DP1);
  x = vmlsq_n_f32(x, y, cephes_DP2);
  x = vmlsq_n_f32(x, y, cephes_DP3);
  // 2）多项式拟合（泰勒级数展开）近似计算sin(x)
  float32x4_t z = vmulq_f32(x, x);
  float32x4_t y1 = vmlaq_n_f32(vdupq_n_f32(cephes_coscof_p1), z, cephes_coscof_p0);
  y1 = vmlaq_f32(vdupq_n_f32(cephes_coscof_p2), y1, z);
  y1 = vmlaq_f32(vdupq_n_f32(neg_half), y1, z);
  y1 = vmlaq_f32(vdupq_n_f32(one), y1, z);
  float32x4_t y2 = vmlaq_n_f32(vdupq_n_f32(cephes_sincof_p1), z, cephes_sincof_p0);
  y2 = vmlaq_f32(vdupq_n_f32(cephes_sincof_p2), y2, z);
  y2 = vmulq_f32(y2, z);
  y2 = vmlaq_f32(x, y2, x);
  y = vbslq_f32(poly_mask, y2, y1);
  // 3）符号位补偿
  return vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(y), sign));
}