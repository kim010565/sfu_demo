#pragma once

#include <arm_neon.h>
#include <cmath>
#include <iostream>

constexpr float MAXNUMF = 3.4028234663852885981170418348451692544e38;
constexpr float MAXLOGF = 88.72283905206835;
constexpr float MINLOGF = -103.278929903431851103;
constexpr float LOG2EF = 1.44269504088896341;
constexpr float SQRTHF = 0.707106781186547524;
constexpr float PIO4F = 0.7853981633974483096;

inline float32x4_t ldexp(const float32x4_t &a, const float32x4_t &exponent) {
  constexpr float bias = 127.0;
  constexpr float limit = 255.0;
  constexpr float zero = 0.0;
  uint32x4_t e = vcvtq_u32_f32(vminq_f32(vmaxq_f32(vaddq_f32(exponent, vdupq_n_f32(bias)), vdupq_n_f32(zero)), vdupq_n_f32(limit)));  // exponent + 127
  return vmulq_f32(a, vreinterpretq_f32_u32(vshlq_n_u32(e, 23)));                                                                     // a * (2^e)
}

inline float32x4_t frexpf(const float32x4_t &a, float32x4_t &exponent) {
  constexpr uint32_t exp_mantissa_mask = 0x7fffffff;
  constexpr uint32_t sign_mantissa_mask = ~0x7f800000;
  constexpr float half = 0.5;
  constexpr float zero = 0.0;
  constexpr uint32_t normal_min = 0x1;
  constexpr float normalization_factor = 16777216.0;
  constexpr float normalization_offset = -24.0;
  constexpr float non_finite_exponent = 255.0;
  // To handle denormals, normalize by multiplying by 2^(int(MantissaBits)+1).
  uint32x4_t is_denormal = vcltq_f32(vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(a), vdupq_n_u32(exp_mantissa_mask))), vdupq_n_f32(*(float *)(&normal_min)));
  float32x4_t normalized_a = vbslq_f32(is_denormal, vmulq_n_f32(a, normalization_factor), a);
  // Determine exponent offset: -126 if normal, -126-24 if denormal
  float32x4_t exponent_offset = vdupq_n_f32(-126.0);
  exponent_offset = vbslq_f32(is_denormal, vaddq_f32(exponent_offset, vdupq_n_f32(normalization_offset)), exponent_offset);
  // Determine exponent and mantissa from normalized_a.
  exponent = vcvtq_f32_u32(vshrq_n_u32(vandq_u32(vreinterpretq_u32_f32(normalized_a), vdupq_n_u32(exp_mantissa_mask)), 23));
  // Zero, Inf and NaN return 'a' unmodified, exponent is zero
  uint32x4_t is_zero_or_not_finite = vorrq_u32(vceqzq_f32(a), vceqq_f32(exponent, vdupq_n_f32(non_finite_exponent)));
  float32x4_t m = vbslq_f32(is_zero_or_not_finite, a, vreinterpretq_f32_u32(vorrq_u32(vandq_u32(vreinterpretq_u32_f32(normalized_a), vdupq_n_u32(sign_mantissa_mask)), vdupq_n_u32(*(uint32_t *)(&half)))));
  exponent = vbslq_f32(is_zero_or_not_finite, vdupq_n_f32(zero), vaddq_f32(exponent, exponent_offset));
  return m;
}
