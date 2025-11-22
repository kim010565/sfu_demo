#include "expf.h"
#include "logf.h"
#include "sfu.h"
#include "sinf.h"

int main(int argc, char **argv) {
  if (1) {
    float a[16] = {-100.0f, -88.0f, -15.5f, -10.0f, -1.5f, -0.25f, -0.0125f, 0.0f, 0.0125f, 0.25f, 1.5f, 10.0f, 15.5f, 50.6f, 88.0f, 100.0f};
    float golden[16];
    float scalar_rst[16];
    float vector_rst[16];
    float sfu_rst[16];

    for (int i = 0; i < 16; i++) {
      // a[i] = 88.0f;
      golden[i] = std::exp(a[i]);
      scalar_rst[i] = expf(a[i]);
      sfu_rst[i] = exp2f_sfu(a[i] * 1.4426950408889634073599246810019f);  // exp(x) = exp2(x * (1/ln(2)))
    }

    __m256 x = _mm256_load_ps(a);
    __m256 y = expf(x);
    _mm256_store_ps(vector_rst, y);
    x = _mm256_load_ps(a + 8);
    y = expf(x);
    _mm256_store_ps(vector_rst + 8, y);

    for (int i = 0; i < 16; i++) {
      printf("x86 expf test: input=%g, golden=%g, scalar_rst=%g, vector_rst=%g, sfu_rst=%g\n", a[i], golden[i], scalar_rst[i], vector_rst[i], sfu_rst[i]);
    }
    printf("\n");
  }

  if (1) {
    float a[16] = {0.0f, 0.0125f, 0.25f, 1.5f, 10.0f, 15.5f, 50.6f, 88.0f, 100.0f, 150.05f, 210.0f, 250.08f, 300.6f, 400.1f, 499.9f, 600.0f};
    float golden[16];
    float scalar_rst[16];
    float vector_rst[16];
    float sfu_rst[16];

    for (int i = 0; i < 16; i++) {
      golden[i] = std::log(a[i]);
      scalar_rst[i] = logf(a[i]);
      sfu_rst[i] = logf_sfu(a[i]);
    }

    __m256 x = _mm256_load_ps(a);
    __m256 y = logf(x);
    _mm256_store_ps(vector_rst, y);
    x = _mm256_load_ps(a + 8);
    y = logf(x);
    _mm256_store_ps(vector_rst + 8, y);

    for (int i = 0; i < 16; i++) {
      printf("x86 logf test: input=%g, golden=%g, scalar_rst=%g, vector_rst=%g, sfu_rst=%g\n", a[i], golden[i], scalar_rst[i], vector_rst[i], sfu_rst[i]);
    }
    printf("\n");
  }

  if (1) {
    float a[16] = {-10.0f, -3.0f, -1.7f, -0.77f, -0.25f, -0.5f, 0.0f, 0.5f, 0.77f, 1.7f, 3.0f, 3.65f, 4.0f, 5.2f, 6.0f, 100.0f};
    float golden[16];
    float scalar_rst[16];
    float vector_rst[16];
    float sfu_rst[16];

    for (int i = 0; i < 16; i++) {
      golden[i] = std::sin(a[i]);
      scalar_rst[i] = sinf(a[i]);
      sfu_rst[i] = sinf_sfu(a[i]);
    }

    __m256 x = _mm256_load_ps(a);
    __m256 y = sinf(x);
    _mm256_store_ps(vector_rst, y);
    x = _mm256_load_ps(a + 8);
    y = sinf(x);
    _mm256_store_ps(vector_rst + 8, y);

    for (int i = 0; i < 16; i++) {
      printf("x86 sinf test: input=%g, golden=%g, scalar_rst=%g, vector_rst=%g, sfu_rst=%g\n", a[i], golden[i], scalar_rst[i], vector_rst[i], sfu_rst[i]);
    }
    printf("\n");
  }

  if (1) {
    float a[16] = {-10.0f, -3.0f, -1.7f, -0.77f, -0.25f, -0.5f, 0.0f, 0.5f, 0.77f, 1.7f, 3.0f, 3.65f, 4.0f, 5.2f, 6.0f, 100.0f};
    float golden[16];
    float scalar_rst[16];
    float vector_rst[16];
    float sfu_rst[16];

    for (int i = 0; i < 16; i++) {
      golden[i] = std::cos(a[i]);
      scalar_rst[i] = cosf(a[i]);
      sfu_rst[i] = cosf_sfu(a[i]);
    }

    __m256 x = _mm256_load_ps(a);
    __m256 y = cosf(x);
    _mm256_store_ps(vector_rst, y);
    x = _mm256_load_ps(a + 8);
    y = cosf(x);
    _mm256_store_ps(vector_rst + 8, y);

    for (int i = 0; i < 16; i++) {
      printf("x86 cosf test: input=%g, golden=%g, scalar_rst=%g, vector_rst=%g, sfu_rst=%g\n", a[i], golden[i], scalar_rst[i], vector_rst[i], sfu_rst[i]);
    }
    printf("\n");
  }

  if (1) {
    float a[16] = {-10.0f, -3.0f, -1.7f, -0.77f, -0.25f, -0.5f, 0.0f, 0.5f, 0.77f, 1.7f, 3.0f, 3.65f, 4.0f, 5.2f, 6.0f, 100.0f};
    float golden[16];
    float vector_rst[16];
    float sfu_rst[16];

    for (int i = 0; i < 16; i++) {
      golden[i] = 1.0f / a[i];
      sfu_rst[i] = rcp_sfu(a[i]);
    }

    __m256 x = _mm256_load_ps(a);
    __m256 y = _mm256_rcp_ps(x);
    y = _mm256_mul_ps(_mm256_fnmadd_ps(x, y, _mm256_set1_ps(2.0)), y);  // 第一次牛顿迭代：y = (2.0 - x * y) * y
    _mm256_store_ps(vector_rst, y);
    x = _mm256_load_ps(a + 8);
    y = _mm256_rcp_ps(x);
    y = _mm256_mul_ps(_mm256_fnmadd_ps(x, y, _mm256_set1_ps(2.0)), y);  // 第一次牛顿迭代：y = (2.0 - x * y) * y
    _mm256_store_ps(vector_rst + 8, y);

    for (int i = 0; i < 16; i++) {
      printf("x86 rcp test: input=%g, golden=%g, vector_rst=%g, sfu_rst=%g\n", a[i], golden[i], vector_rst[i], sfu_rst[i]);
    }
    printf("\n");
  }

  return 0;
}