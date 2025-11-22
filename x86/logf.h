#pragma once

#include "common.h"

inline float logf(float xx) {
  float x = xx;
  // 输入范围保护
  if (x <= 0.0) {
    return (MINLOGF);
  }
  // 1) 缩小范围
  // log(x) = log(2^e * m) = e*log(2) + log(m) = e*log(2) + log(1+r), 缩小范围后的r在[sqrt(1/2)-1, sqrt(2)-1)内
  int e;
  x = frexpf(x, &e);  // 将单精度浮点数分解为归一化尾数和指数。x = sign*2^e*m，其中，m是归一化尾数，范围在[0.5,1)；e是指数
  if (x < SQRTHF) {   // 把r的范围移动到[sqrt(1/2)-1, sqrt(2)-1)（The values are then centered around 0, which improves the stability of the polynomial evaluation）
    e -= 1;
    x = x + x - 1.0;  // r = [0, sqrt(2)-1)
  } else {
    x = x - 1.0;  // r = [sqrt(1/2)-1, 0)
  }
  // 多项式拟合（泰勒级数展开）近似计算log(1+r)
  float z = x * x;
  float y = ((((((((7.0376836292E-2 * x - 1.1514610310E-1) * x + 1.1676998740E-1) * x - 1.2420140846E-1) * x + 1.4249322787E-1) * x - 1.6668057665E-1) * x + 2.0000714765E-1) * x - 2.4999993993E-1) * x + 3.3333331174E-1) * x * z;
  float fe = 0.0;
  if (e) {
    fe = e;
    y += -2.12194440e-4 * fe;
  }
  y += -0.5 * z;
  z = x + y;
  // 3）恢复范围，计算e*log(2) + log(1+r)
  if (e) {
    z += 0.693359375 * fe;  // 这里把log2分成了0.693359375和-2.12194440e-4两部分
  }
  return (z);
}

inline __m256 logf(__m256 xx) {
  const __m256 min_norm_pos = _mm256_castsi256_ps(_mm256_set1_epi32(0x00800000u));
  const __m256 minus_inf = _mm256_castsi256_ps(_mm256_set1_epi32(0xff800000u));
  const __m256 pos_inf = _mm256_castsi256_ps(_mm256_set1_epi32(0x7f800000u));
  const __m256 cephes_SQRTHF = _mm256_set1_ps(SQRTHF);
  const __m256 cephes_log_p0 = _mm256_set1_ps(7.0376836292E-2);
  const __m256 cephes_log_p1 = _mm256_set1_ps(-1.1514610310E-1);
  const __m256 cephes_log_p2 = _mm256_set1_ps(1.1676998740E-1);
  const __m256 cephes_log_p3 = _mm256_set1_ps(-1.2420140846E-1);
  const __m256 cephes_log_p4 = _mm256_set1_ps(1.4249322787E-1);
  const __m256 cephes_log_p5 = _mm256_set1_ps(-1.6668057665E-1);
  const __m256 cephes_log_p6 = _mm256_set1_ps(2.0000714765E-1);
  const __m256 cephes_log_p7 = _mm256_set1_ps(-2.4999993993E-1);
  const __m256 cephes_log_p8 = _mm256_set1_ps(3.3333331174E-1);
  const __m256 zero = _mm256_setzero_ps();
  const __m256 one = _mm256_set1_ps(1.0);
  const __m256 neg_half = _mm256_set1_ps(-0.5);
  const __m256 cephes_log2 = _mm256_set1_ps(0.69314718055994530941723212145818);
  // 输入范围保护
  __m256 x = _mm256_max_ps(xx, min_norm_pos);
  // 1) 缩小范围
  __m256 e;
  x = frexpf(x, e);
  __m256 mask = _mm256_cmp_ps(x, cephes_SQRTHF, _CMP_LT_OS);
  __m256 tmp = _mm256_and_ps(x, mask);
  x = _mm256_sub_ps(x, one);
  e = _mm256_sub_ps(e, _mm256_and_ps(mask, one));
  x = _mm256_add_ps(x, tmp);
  // 多项式拟合（泰勒级数展开）近似计算log(1+r)
  __m256 z = _mm256_mul_ps(x, x);
  __m256 x3 = _mm256_mul_ps(x, z);
  __m256 y = _mm256_fmadd_ps(cephes_log_p0, x, cephes_log_p1);
  __m256 y1 = _mm256_fmadd_ps(cephes_log_p3, x, cephes_log_p4);
  __m256 y2 = _mm256_fmadd_ps(cephes_log_p6, x, cephes_log_p7);
  y = _mm256_fmadd_ps(y, x, cephes_log_p2);
  y1 = _mm256_fmadd_ps(y1, x, cephes_log_p5);
  y2 = _mm256_fmadd_ps(y2, x, cephes_log_p8);
  y = _mm256_fmadd_ps(y, x3, y1);
  y = _mm256_fmadd_ps(y, x3, y2);
  y = _mm256_mul_ps(y, x3);
  y = _mm256_fmadd_ps(neg_half, z, y);
  z = _mm256_add_ps(x, y);
  // 3）恢复范围，计算e*log(2) + log(1+r)
  z = _mm256_fmadd_ps(cephes_log2, e, z);
  __m256 invalid_mask = _mm256_cmp_ps(xx, zero, _CMP_NGE_UQ);
  __m256 iszero_mask = _mm256_cmp_ps(xx, zero, _CMP_EQ_OQ);
  __m256 pos_inf_mask = _mm256_cmp_ps(xx, pos_inf, _CMP_EQ_OQ);
  return _mm256_blendv_ps(_mm256_or_ps(_mm256_blendv_ps(z, pos_inf, pos_inf_mask), invalid_mask), minus_inf, iszero_mask);
}