#pragma once

#include "common.h"

constexpr float C1 = 0.693359375;
constexpr float C2 = -2.12194440e-4;

inline float expf(float xx) {
  float x = xx;
  if (x > MAXLOGF) {  // 上限保护
    return (MAXNUMF);
  }

  if (x < MINLOGF) {  // 下限保护
    return (0.0);
  }
  // 1) 缩小范围
  // 把x表示为：x = g + n * log2
  // 那么，e^x = e^g * e^(n * log2) = e^g * 2^n
  // n = floor(x/log(2) + 0.5); g = x - n * log2, 缩小范围后的g在[-0.5. +0.5]内
  float z = floorf(LOG2EF * x + 0.5);
  x -= z * C1;
  x -= z * C2;  // C1、C2对应log2，这里为了防止accumulating truncation errors，拆分成了2部分（如果对精度要求不高，也可以合并）
  int n = z;
  // 2）多项式拟合（泰勒级数展开）近似计算e^g
  z = x * x;
  z = (((((1.9875691500E-4 * x + 1.3981999507E-3) * x + 8.3334519073E-3) * x + 4.1665795894E-2) * x + 1.6666665459E-1) * x + 5.0000001201E-1) * z + x + 1.0;
  // 3）恢复范围，计算e^g * 2^n
  x = ldexpf(z, n);
  return (x);
}

inline __m256 expf(__m256 xx) {
  const __m256 exp_hi = _mm256_set1_ps(MAXLOGF);
  const __m256 exp_lo = _mm256_set1_ps(-MAXLOGF);
  const __m256 cephes_LOG2EF = _mm256_set1_ps(LOG2EF);
  const __m256 cephes_C1 = _mm256_set1_ps(C1);
  const __m256 cephes_C2 = _mm256_set1_ps(C2);
  const __m256 cephes_exp_p0 = _mm256_set1_ps(1.9875691500E-4);
  const __m256 cephes_exp_p1 = _mm256_set1_ps(1.3981999507E-3);
  const __m256 cephes_exp_p2 = _mm256_set1_ps(8.3334519073E-3);
  const __m256 cephes_exp_p3 = _mm256_set1_ps(4.1665795894E-2);
  const __m256 cephes_exp_p4 = _mm256_set1_ps(1.6666665459E-1);
  const __m256 cephes_exp_p5 = _mm256_set1_ps(5.0000001201E-1);
  const __m256 one = _mm256_set1_ps(1.0);
  const __m256 half = _mm256_set1_ps(0.5);
  // 上限、下限保护
  __m256 x = _mm256_max_ps(_mm256_min_ps(xx, exp_hi), exp_lo);
  // 1) 缩小范围
  __m256 z = _mm256_floor_ps(_mm256_fmadd_ps(cephes_LOG2EF, x, half));
  x = _mm256_fnmadd_ps(z, cephes_C1, x);
  x = _mm256_fnmadd_ps(z, cephes_C2, x);
  __m256 n = z;
  // 2）多项式拟合（泰勒级数展开）近似计算e^g
  z = _mm256_mul_ps(x, x);
  __m256 y = _mm256_fmadd_ps(cephes_exp_p0, x, cephes_exp_p1);
  __m256 y1 = _mm256_fmadd_ps(cephes_exp_p3, x, cephes_exp_p4);
  __m256 y2 = _mm256_add_ps(x, one);
  y = _mm256_fmadd_ps(y, x, cephes_exp_p2);
  y1 = _mm256_fmadd_ps(y1, x, cephes_exp_p5);
  y = _mm256_fmadd_ps(y, _mm256_mul_ps(z, x), y1);
  y = _mm256_fmadd_ps(y, z, y2);
  // 3）恢复范围，计算e^g * 2^n
  return _mm256_max_ps(ldexp(y, n), xx);
}