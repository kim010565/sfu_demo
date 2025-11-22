#pragma once

#include "common.h"

constexpr float FOPI = 1.27323954473516;
constexpr float DP1 = 0.78515625;
constexpr float DP2 = 2.4187564849853515625e-4;
constexpr float DP3 = 3.77489497744594108e-8;
constexpr float lossth = 8192.;
constexpr float T24M1 = 16777215.;

inline float sinf(float xx) {
  int sign = 1;
  float x = xx;
  if (xx < 0) {  // 取绝对值
    sign = -1;
    x = -xx;
  }
  if (x > T24M1) {  // 输入范围保护
    return (0.0);
  }
  // 1）缩小范围
  // 把一个圆平均分成8份，根据sin性质，缩小范围到[0,pi/4)或[pi/2,3*pi/4)
  int j = FOPI * x;  // 即j=floor(x/(PI/4))。把[0,2*pi)平均分成8个区间，j表示落在哪个区间
  float y = j;
  if (j & 1) {  // 奇数->偶数。即仅拟合区间0、2、4、6部分（[0,pi/4)、[pi/2,3*pi/4)、[pi,5*pi/4)、[3*pi/2,7*pi/4)）
    j += 1;
    y += 1.0;
  }
  j &= 7;       // 对应性质：sin(x) = sin(x + 2*pi)
  if (j > 3) {  // 对应性质：sin(x) = -sin(x + pi)，即仅拟合区间0、2部分（[0,pi/4)、[pi/2,3*pi/4)）
    sign = -sign;
    j -= 4;
  }
  if (x > lossth) {
    x = x - y * PIO4F;
  } else {
    x = ((x - y * DP1) - y * DP2) - y * DP3;  // DP1+DP2+DP3 = PIO4F = pi/4，这里是为了防止accumulating truncation errors，拆分成了3部分（如果对精度要求不高，也可以合并）
  }
  // 2）多项式拟合（泰勒级数展开）近似计算sin(x)
  float z = x * x;
  if (j == 2) {  // 拟合[pi/2,3*pi/4)区间，cos(x)=sin(x+pi/2)，所以，对应cos系数
    y = ((2.443315711809948E-005 * z - 1.388731625493765E-003) * z + 4.166664568298827E-002) * z * z;
    y -= 0.5 * z;
    y += 1.0;
  } else {  // 拟合[0,pi/4)区间，所以，对应sin系数
    y = (((-1.9515295891E-4 * z + 8.3321608736E-3) * z - 1.6666654611E-1) * z * x) + x;
  }
  // 3）符号位补偿
  if (sign < 0) {
    y = -y;
  }
  return (y);
}

inline float cosf(float xx) {
  int sign = 1;
  float x = xx;
  if (x < 0) {  // 取绝对值
    x = -x;
  }
  if (x > T24M1) {  // 输入范围保护
    return (0.0);
  }
  // 1）缩小范围
  // 原理类似于sinf
  int j = FOPI * x;
  float y = j;
  if (j & 1) {
    j += 1;
    y += 1.0;
  }
  j &= 7;
  if (j > 3) {
    sign = -sign;
    j -= 4;
  }
  if (j > 1) {
    sign = -sign;
  }
  if (x > lossth) {
    x = x - y * PIO4F;
  } else {
    x = ((x - y * DP1) - y * DP2) - y * DP3;
  }
  // 2）多项式拟合（泰勒级数展开）近似计算cos(x)
  float z = x * x;
  if (j == 2) {
    y = (((-1.9515295891E-4 * z + 8.3321608736E-3) * z - 1.6666654611E-1) * z * x) + x;
  } else {
    y = ((2.443315711809948E-005 * z - 1.388731625493765E-003) * z + 4.166664568298827E-002) * z * z;
    y -= 0.5 * z;
    y += 1.0;
  }
  if (sign < 0) {
    y = -y;
  }
  return (y);
}

inline __m256 sinf(__m256 xx) {
  const __m256 sign_mask = _mm256_castsi256_ps(_mm256_set1_epi32(0x80000000u));
  const __m256 exp_mantissa_mask = _mm256_castsi256_ps(_mm256_set1_epi32(0x7fffffff));
  const __m256 cephes_FOPI = _mm256_set1_ps(FOPI);
  const __m256 cephes_DP1 = _mm256_set1_ps(DP1);
  const __m256 cephes_DP2 = _mm256_set1_ps(DP2);
  const __m256 cephes_DP3 = _mm256_set1_ps(DP3);
  const __m256 cephes_coscof_p0 = _mm256_set1_ps(2.443315711809948E-005);
  const __m256 cephes_coscof_p1 = _mm256_set1_ps(-1.388731625493765E-003);
  const __m256 cephes_coscof_p2 = _mm256_set1_ps(4.166664568298827E-002);
  const __m256 cephes_sincof_p0 = _mm256_set1_ps(-1.9515295891E-4);
  const __m256 cephes_sincof_p1 = _mm256_set1_ps(8.3321608736E-3);
  const __m256 cephes_sincof_p2 = _mm256_set1_ps(-1.6666654611E-1);
  const __m256 neg_half = _mm256_set1_ps(-0.5);
  const __m256 one = _mm256_set1_ps(1.0);
  const __m256i one_32 = _mm256_set1_epi32(1);
  const __m256i two_32 = _mm256_set1_epi32(2);
  const __m256i four_32 = _mm256_set1_epi32(4);
  const __m256i inv_one_32 = _mm256_set1_epi32(~1);
  __m256 x = _mm256_and_ps(xx, exp_mantissa_mask);  // 取绝对值
  __m256 sign = _mm256_and_ps(xx, sign_mask);
  // 1）缩小范围
  __m256 y = _mm256_mul_ps(x, cephes_FOPI);
  __m256i j = _mm256_cvttps_epi32(y);
  j = _mm256_and_si256(_mm256_add_epi32(j, one_32), inv_one_32);
  y = _mm256_cvtepi32_ps(j);
  __m256 poly_mask = _mm256_castsi256_ps(_mm256_cmpeq_epi32(two_32, _mm256_and_si256(j, two_32)));
  sign = _mm256_xor_ps(sign, _mm256_castsi256_ps(_mm256_slli_epi32(_mm256_and_si256(j, four_32), 29)));
  x = _mm256_fnmadd_ps(y, cephes_DP1, x);
  x = _mm256_fnmadd_ps(y, cephes_DP2, x);
  x = _mm256_fnmadd_ps(y, cephes_DP3, x);
  // 2）多项式拟合（泰勒级数展开）近似计算sin(x)
  __m256 z = _mm256_mul_ps(x, x);
  __m256 y1 = _mm256_fmadd_ps(cephes_coscof_p0, z, cephes_coscof_p1);
  y1 = _mm256_fmadd_ps(y1, z, cephes_coscof_p2);
  y1 = _mm256_fmadd_ps(y1, z, neg_half);
  y1 = _mm256_fmadd_ps(y1, z, one);
  __m256 y2 = _mm256_fmadd_ps(cephes_sincof_p0, z, cephes_sincof_p1);
  y2 = _mm256_fmadd_ps(y2, z, cephes_sincof_p2);
  y2 = _mm256_mul_ps(y2, z);
  y2 = _mm256_fmadd_ps(y2, x, x);
  y = _mm256_blendv_ps(y2, y1, poly_mask);
  // 3）符号位补偿
  return _mm256_xor_ps(y, sign);
}

inline __m256 cosf(__m256 xx) {
  const __m256 sign_mask = _mm256_castsi256_ps(_mm256_set1_epi32(0x80000000u));
  const __m256 exp_mantissa_mask = _mm256_castsi256_ps(_mm256_set1_epi32(0x7fffffff));
  const __m256 cephes_FOPI = _mm256_set1_ps(FOPI);
  const __m256 cephes_DP1 = _mm256_set1_ps(DP1);
  const __m256 cephes_DP2 = _mm256_set1_ps(DP2);
  const __m256 cephes_DP3 = _mm256_set1_ps(DP3);
  const __m256 cephes_coscof_p0 = _mm256_set1_ps(2.443315711809948E-005);
  const __m256 cephes_coscof_p1 = _mm256_set1_ps(-1.388731625493765E-003);
  const __m256 cephes_coscof_p2 = _mm256_set1_ps(4.166664568298827E-002);
  const __m256 cephes_sincof_p0 = _mm256_set1_ps(-1.9515295891E-4);
  const __m256 cephes_sincof_p1 = _mm256_set1_ps(8.3321608736E-3);
  const __m256 cephes_sincof_p2 = _mm256_set1_ps(-1.6666654611E-1);
  const __m256 neg_half = _mm256_set1_ps(-0.5);
  const __m256 one = _mm256_set1_ps(1.0);
  const __m256i one_32 = _mm256_set1_epi32(1);
  const __m256i two_32 = _mm256_set1_epi32(2);
  const __m256i four_32 = _mm256_set1_epi32(4);
  const __m256i inv_one_32 = _mm256_set1_epi32(~1);
  __m256 x = _mm256_and_ps(xx, exp_mantissa_mask);  // 取绝对值
  // 1）缩小范围
  __m256 y = _mm256_mul_ps(x, cephes_FOPI);
  __m256i j = _mm256_cvttps_epi32(y);
  j = _mm256_and_si256(_mm256_add_epi32(j, one_32), inv_one_32);
  y = _mm256_cvtepi32_ps(j);
  __m256 poly_mask = _mm256_castsi256_ps(_mm256_cmpeq_epi32(two_32, _mm256_and_si256(j, two_32)));
  __m256 sign = _mm256_xor_ps(_mm256_castsi256_ps(_mm256_slli_epi32(_mm256_and_si256(j, four_32), 29)), _mm256_castsi256_ps(_mm256_slli_epi32(_mm256_and_si256(j, two_32), 30)));
  x = _mm256_fnmadd_ps(y, cephes_DP1, x);
  x = _mm256_fnmadd_ps(y, cephes_DP2, x);
  x = _mm256_fnmadd_ps(y, cephes_DP3, x);
  // 2）多项式拟合（泰勒级数展开）近似计算sin(x)
  __m256 z = _mm256_mul_ps(x, x);
  __m256 y1 = _mm256_fmadd_ps(cephes_coscof_p0, z, cephes_coscof_p1);
  y1 = _mm256_fmadd_ps(y1, z, cephes_coscof_p2);
  y1 = _mm256_fmadd_ps(y1, z, neg_half);
  y1 = _mm256_fmadd_ps(y1, z, one);
  __m256 y2 = _mm256_fmadd_ps(cephes_sincof_p0, z, cephes_sincof_p1);
  y2 = _mm256_fmadd_ps(y2, z, cephes_sincof_p2);
  y2 = _mm256_mul_ps(y2, z);
  y2 = _mm256_fmadd_ps(y2, x, x);
  y = _mm256_blendv_ps(y1, y2, poly_mask);
  // 3）符号位补偿
  return _mm256_xor_ps(y, sign);
}