#pragma once

#include <riscv_vector.h>
#include <cmath>
#include <iostream>

constexpr float MAXNUMF = 3.4028234663852885981170418348451692544e38;
constexpr float MAXLOGF = 88.72283905206835;
constexpr float MINLOGF = -103.278929903431851103;
constexpr float LOG2EF = 1.44269504088896341;
constexpr float SQRTHF = 0.707106781186547524;
constexpr float PIO4F = 0.7853981633974483096;

inline vfloat32m4_t ldexp(const vfloat32m4_t &a, const vfloat32m4_t &exponent, size_t vl) {
  constexpr float bias = 127.0;
  constexpr float limit = 255.0;
  constexpr float zero = 0.0;
  vuint32m4_t e = __riscv_vfcvt_xu_f_v_u32m4(__riscv_vfmin_vf_f32m4(__riscv_vfmax_vf_f32m4(__riscv_vfadd_vf_f32m4(exponent, bias, vl), zero, vl), limit, vl), vl);  // exponent + 127
  return __riscv_vfmul_vv_f32m4(a, __riscv_vreinterpret_v_u32m4_f32m4(__riscv_vsll_vx_u32m4(e, 23, vl)), vl);                                    // a * (2^e)
}

inline vfloat32m4_t frexpf(const vfloat32m4_t &a, vfloat32m4_t &exponent, size_t vl) {
  constexpr uint32_t exp_mantissa_mask = 0x7fffffff;
  constexpr uint32_t sign_mantissa_mask = ~0x7f800000;
  constexpr float half = 0.5;
  constexpr float zero = 0.0;
  constexpr uint32_t normal_min = 0x1;
  constexpr float normalization_factor = 16777216.0;
  constexpr float normalization_offset = -24.0;
  constexpr float non_finite_exponent = 255.0;
  // To handle denormals, normalize by multiplying by 2^(int(MantissaBits)+1).
  vbool8_t is_denormal = __riscv_vmflt_vf_f32m4_b8(__riscv_vreinterpret_v_u32m4_f32m4(__riscv_vand_vx_u32m4(__riscv_vreinterpret_v_f32m4_u32m4(a), exp_mantissa_mask, vl)), *(float*)(&normal_min), vl);
  vfloat32m4_t normalized_a = __riscv_vmerge_vvm_f32m4(a, __riscv_vfmul_vf_f32m4(a, normalization_factor, vl), is_denormal, vl);
  // Determine exponent offset: -126 if normal, -126-24 if denormal
  vfloat32m4_t exponent_offset = __riscv_vfmv_v_f_f32m4(-126.0, vl);
  exponent_offset = __riscv_vfadd_vf_f32m4_m(is_denormal, exponent_offset, normalization_offset, vl);
  // Determine exponent and mantissa from normalized_a.
  exponent = __riscv_vfcvt_f_xu_v_f32m4(__riscv_vsrl_vx_u32m4(__riscv_vand_vx_u32m4(__riscv_vreinterpret_v_f32m4_u32m4(normalized_a), exp_mantissa_mask, vl), 23, vl), vl);
  // Zero, Inf and NaN return 'a' unmodified, exponent is zero
  vbool8_t is_zero_or_not_finite = __riscv_vmor_mm_b8(__riscv_vmfeq_vf_f32m4_b8(a, zero, vl), __riscv_vmfeq_vf_f32m4_b8(exponent, non_finite_exponent, vl), vl);
  vfloat32m4_t m = __riscv_vmerge_vvm_f32m4(__riscv_vreinterpret_v_u32m4_f32m4(__riscv_vor_vx_u32m4(__riscv_vand_vx_u32m4(__riscv_vreinterpret_v_f32m4_u32m4(normalized_a), sign_mantissa_mask, vl), *(uint32_t*)(&half), vl)), a, is_zero_or_not_finite, vl);
  exponent = __riscv_vfmerge_vfm_f32m4(__riscv_vfadd_vv_f32m4(exponent, exponent_offset, vl), zero, is_zero_or_not_finite, vl);
  return m;
}
