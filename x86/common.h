#pragma once

#include <emmintrin.h>
#include <immintrin.h>
#include <cmath>
#include <iostream>

constexpr float MAXNUMF = 3.4028234663852885981170418348451692544e38;
constexpr float MAXLOGF = 88.72283905206835;
constexpr float MINLOGF = -103.278929903431851103;
constexpr float LOG2EF = 1.44269504088896341;
constexpr float SQRTHF = 0.707106781186547524;
constexpr float PIO4F = 0.7853981633974483096;

inline __m256 ldexp(const __m256 &a, const __m256 &exponent) {
  const __m256 bias = _mm256_set1_ps(127.0);
  const __m256 limit = _mm256_set1_ps(255.0);
  const __m256 zero = _mm256_setzero_ps();
  __m256i e = _mm256_cvtps_epi32(_mm256_min_ps(_mm256_max_ps(_mm256_add_ps(exponent, bias), zero), limit));  // exponent + 127
  return _mm256_mul_ps(a, _mm256_castsi256_ps(_mm256_slli_epi32(e, 23)));                                    // a * (2^e)
}

inline __m256 frexpf(const __m256 &a, __m256 &exponent) {
  const __m256 exp_mantissa_mask = _mm256_castsi256_ps(_mm256_set1_epi32(0x7fffffff));
  const __m256 sign_mantissa_mask = _mm256_castsi256_ps(_mm256_set1_epi32(~0x7f800000));
  const __m256 half = _mm256_set1_ps(0.5);
  const __m256 zero = _mm256_setzero_ps();
  const __m256 normal_min = _mm256_castsi256_ps(_mm256_set1_epi32(0x1));
  const __m256 normalization_factor = _mm256_set1_ps(16777216.0);
  const __m256 normalization_offset = _mm256_set1_ps(-24.0);
  const __m256 non_finite_exponent = _mm256_set1_ps(255.0);
  // To handle denormals, normalize by multiplying by 2^(int(MantissaBits)+1).
  __m256 is_denormal = _mm256_cmp_ps(_mm256_and_ps(a, exp_mantissa_mask), normal_min, _CMP_LT_OS);
  __m256 normalized_a = _mm256_blendv_ps(a, _mm256_mul_ps(a, normalization_factor), is_denormal);
  // Determine exponent offset: -126 if normal, -126-24 if denormal
  __m256 exponent_offset = _mm256_set1_ps(-126.0);
  exponent_offset = _mm256_blendv_ps(exponent_offset, _mm256_add_ps(exponent_offset, normalization_offset), is_denormal);
  // Determine exponent and mantissa from normalized_a.
  exponent = _mm256_cvtepi32_ps(_mm256_srli_epi32(_mm256_castps_si256(_mm256_and_ps(normalized_a, exp_mantissa_mask)), 23));
  // Zero, Inf and NaN return 'a' unmodified, exponent is zero
  __m256 is_zero_or_not_finite = _mm256_or_ps(_mm256_cmp_ps(a, zero, _CMP_EQ_OS), _mm256_cmp_ps(exponent, non_finite_exponent, _CMP_EQ_OS));
  __m256 m = _mm256_blendv_ps(_mm256_or_ps(_mm256_and_ps(normalized_a, sign_mantissa_mask), half), a, is_zero_or_not_finite);
  exponent = _mm256_blendv_ps(_mm256_add_ps(exponent, exponent_offset), zero, is_zero_or_not_finite);
  return m;
}
