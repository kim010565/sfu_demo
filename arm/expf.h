#pragma once

#include "common.h"

constexpr float C1 = 0.693359375;
constexpr float C2 = -2.12194440e-4;

inline float32x4_t expf(float32x4_t xx) {
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
  float32x4_t x = vmaxq_f32(vminq_f32(xx, vdupq_n_f32(exp_hi)), vdupq_n_f32(exp_lo));
  // 1) 缩小范围
  float32x4_t z = vcvtq_f32_s32(vcvtq_s32_f32(vmlaq_n_f32(vdupq_n_f32(half), x, cephes_LOG2EF)));
  x = vmlsq_n_f32(x, z, cephes_C1);
  x = vmlsq_n_f32(x, z, cephes_C2);
  float32x4_t n = z;
  // 2）多项式拟合（泰勒级数展开）近似计算e^g
  z = vmulq_f32(x, x);
  float32x4_t y = vmlaq_n_f32(vdupq_n_f32(cephes_exp_p1), x, cephes_exp_p0);
  float32x4_t y1 = vmlaq_n_f32(vdupq_n_f32(cephes_exp_p4), x, cephes_exp_p3);
  float32x4_t y2 = vaddq_f32(x, vdupq_n_f32(one));
  y = vmlaq_f32(vdupq_n_f32(cephes_exp_p2), y, x);
  y1 = vmlaq_f32(vdupq_n_f32(cephes_exp_p5), y1, x);
  y = vmlaq_f32(y1, y, vmulq_f32(z, x));
  y = vmlaq_f32(y2, y, z);
  // 3）恢复范围，计算e^g * 2^n
  return vmaxq_f32(ldexp(y, n), xx);
}