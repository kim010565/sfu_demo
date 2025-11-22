#pragma once

#include "common.h"

inline float32x4_t logf(float32x4_t xx) {
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
  float32x4_t x = vmaxq_f32(xx, vdupq_n_f32(*(float *)(&min_norm_pos)));
  // 1) 缩小范围
  float32x4_t e;
  x = frexpf(x, e);
  uint32x4_t mask = vcltq_f32(x, vdupq_n_f32(cephes_SQRTHF));
  float32x4_t tmp = vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(x), mask));
  x = vsubq_f32(x, vdupq_n_f32(one));
  e = vsubq_f32(e, vreinterpretq_f32_u32(vandq_u32(mask, vdupq_n_u32(*(uint32_t *)(&one)))));
  x = vaddq_f32(x, tmp);
  // 多项式拟合（泰勒级数展开）近似计算log(1+r)
  float32x4_t z = vmulq_f32(x, x);
  float32x4_t x3 = vmulq_f32(x, z);
  float32x4_t y = vmlaq_n_f32(vdupq_n_f32(cephes_log_p1), x, cephes_log_p0);
  float32x4_t y1 = vmlaq_n_f32(vdupq_n_f32(cephes_log_p4), x, cephes_log_p3);
  float32x4_t y2 = vmlaq_n_f32(vdupq_n_f32(cephes_log_p7), x, cephes_log_p6);
  y = vmlaq_f32(vdupq_n_f32(cephes_log_p2), y, x);
  y1 = vmlaq_f32(vdupq_n_f32(cephes_log_p5), y1, x);
  y2 = vmlaq_f32(vdupq_n_f32(cephes_log_p8), y2, x);
  y = vmlaq_f32(y1, y, x3);
  y = vmlaq_f32(y2, y, x3);
  y = vmulq_f32(y, x3);
  y = vmlaq_n_f32(y, z, neg_half);
  z = vaddq_f32(x, y);
  // 3）恢复范围，计算e*log(2) + log(1+r)
  z = vmlaq_n_f32(z, e, cephes_log2);
  uint32x4_t invalid_mask = vcltq_f32(xx, vdupq_n_f32(zero));
  uint32x4_t iszero_mask = vceqzq_f32(xx);
  uint32x4_t pos_inf_mask = vceqq_f32(xx, vdupq_n_f32(*(float *)(&pos_inf)));
  return vbslq_f32(iszero_mask, vdupq_n_f32(*(float *)(&minus_inf)), vreinterpretq_f32_u32(vorrq_u32(vreinterpretq_u32_f32(vbslq_f32(pos_inf_mask, vdupq_n_f32(*(float *)(&pos_inf)), z)), invalid_mask)));
}