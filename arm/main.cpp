#include "expf.h"
#include "logf.h"
#include "sinf.h"

int main(int argc, char **argv) {
  if (1) {
    float a[16] = {-100.0f, -88.0f, -15.5f, -10.0f, -1.5f, -0.25f, -0.0125f, 0.0f, 0.0125f, 0.25f, 1.5f, 10.0f, 15.5f, 50.6f, 88.0f, 100.0f};
    float golden[16];
    float vector_rst[16];

    for (int i = 0; i < 16; i++) {
      golden[i] = std::exp(a[i]);
    }

    float32x4_t x = vld1q_f32(a);
    float32x4_t y = expf(x);
    vst1q_f32(vector_rst, y);
    x = vld1q_f32(a + 4);
    y = expf(x);
    vst1q_f32(vector_rst + 4, y);
    x = vld1q_f32(a + 8);
    y = expf(x);
    vst1q_f32(vector_rst + 8, y);
    x = vld1q_f32(a + 12);
    y = expf(x);
    vst1q_f32(vector_rst + 12, y);

    for (int i = 0; i < 16; i++) {
      printf("neon expf test: input=%g, golden=%g, vector_rst=%g\n", a[i], golden[i], vector_rst[i]);
    }
    printf("\n");
  }

  if (1) {
    float a[16] = {0.0f, 0.0125f, 0.25f, 1.5f, 10.0f, 15.5f, 50.6f, 88.0f, 100.0f, 150.05f, 210.0f, 250.08f, 300.6f, 400.1f, 499.9f, 600.0f};
    float golden[16];
    float vector_rst[16];

    for (int i = 0; i < 16; i++) {
      golden[i] = std::log(a[i]);
    }

    float32x4_t x = vld1q_f32(a);
    float32x4_t y = logf(x);
    vst1q_f32(vector_rst, y);
    x = vld1q_f32(a + 4);
    y = logf(x);
    vst1q_f32(vector_rst + 4, y);
    x = vld1q_f32(a + 8);
    y = logf(x);
    vst1q_f32(vector_rst + 8, y);
    x = vld1q_f32(a + 12);
    y = logf(x);
    vst1q_f32(vector_rst + 12, y);

    for (int i = 0; i < 16; i++) {
      printf("neon logf test: input=%g, golden=%g, vector_rst=%g\n", a[i], golden[i], vector_rst[i]);
    }
    printf("\n");
  }

  if (1) {
    float a[16] = {-10.0f, -3.0f, -1.7f, -0.77f, -0.25f, -0.5f, 0.0f, 0.5f, 0.77f, 1.7f, 3.0f, 3.65f, 4.0f, 5.2f, 6.0f, 100.0f};
    float golden[16];
    float vector_rst[16];

    for (int i = 0; i < 16; i++) {
      golden[i] = std::sin(a[i]);
    }

    float32x4_t x = vld1q_f32(a);
    float32x4_t y = sinf(x);
    vst1q_f32(vector_rst, y);
    x = vld1q_f32(a + 4);
    y = sinf(x);
    vst1q_f32(vector_rst + 4, y);
    x = vld1q_f32(a + 8);
    y = sinf(x);
    vst1q_f32(vector_rst + 8, y);
    x = vld1q_f32(a + 12);
    y = sinf(x);
    vst1q_f32(vector_rst + 12, y);

    for (int i = 0; i < 16; i++) {
      printf("neon sinf test: input=%g, golden=%g, vector_rst=%g\n", a[i], golden[i], vector_rst[i]);
    }
    printf("\n");
  }

  if (1) {
    float a[16] = {-10.0f, -3.0f, -1.7f, -0.77f, -0.25f, -0.5f, 0.0f, 0.5f, 0.77f, 1.7f, 3.0f, 3.65f, 4.0f, 5.2f, 6.0f, 100.0f};
    float golden[16];
    float vector_rst[16];

    for (int i = 0; i < 16; i++) {
      golden[i] = std::cos(a[i]);
    }

    float32x4_t x = vld1q_f32(a);
    float32x4_t y = cosf(x);
    vst1q_f32(vector_rst, y);
    x = vld1q_f32(a + 4);
    y = cosf(x);
    vst1q_f32(vector_rst + 4, y);
    x = vld1q_f32(a + 8);
    y = cosf(x);
    vst1q_f32(vector_rst + 8, y);
    x = vld1q_f32(a + 12);
    y = cosf(x);
    vst1q_f32(vector_rst + 12, y);

    for (int i = 0; i < 16; i++) {
      printf("neon cosf test: input=%g, golden=%g, vector_rst=%g\n", a[i], golden[i], vector_rst[i]);
    }
    printf("\n");
  }

  if (1) {
    float a[16] = {-10.0f, -3.0f, -1.7f, -0.77f, -0.25f, -0.5f, 0.0f, 0.5f, 0.77f, 1.7f, 3.0f, 3.65f, 4.0f, 5.2f, 6.0f, 100.0f};
    float golden[16];
    float vector_rst[16];

    for (int i = 0; i < 16; i++) {
      golden[i] = 1.0f / a[i];
    }

    float32x4_t x = vld1q_f32(a);
    float32x4_t y = vrecpeq_f32(x);       // 近似值: y = 1/x
    y = vmulq_f32(vrecpsq_f32(x, y), y);  // 第一次牛顿迭代
    y = vmulq_f32(vrecpsq_f32(x, y), y);  // 第二次牛顿迭代
    vst1q_f32(vector_rst, y);
    x = vld1q_f32(a + 4);
    y = vrecpeq_f32(x);                   // 近似值: y = 1/x
    y = vmulq_f32(vrecpsq_f32(x, y), y);  // 第一次牛顿迭代
    y = vmulq_f32(vrecpsq_f32(x, y), y);  // 第二次牛顿迭代
    vst1q_f32(vector_rst + 4, y);
    x = vld1q_f32(a + 8);
    y = vrecpeq_f32(x);                   // 近似值: y = 1/x
    y = vmulq_f32(vrecpsq_f32(x, y), y);  // 第一次牛顿迭代
    y = vmulq_f32(vrecpsq_f32(x, y), y);  // 第二次牛顿迭代
    vst1q_f32(vector_rst + 8, y);
    x = vld1q_f32(a + 12);
    y = vrecpeq_f32(x);                   // 近似值: y = 1/x
    y = vmulq_f32(vrecpsq_f32(x, y), y);  // 第一次牛顿迭代
    y = vmulq_f32(vrecpsq_f32(x, y), y);  // 第二次牛顿迭代
    vst1q_f32(vector_rst + 12, y);

    for (int i = 0; i < 16; i++) {
      printf("neon rcp test: input=%g, golden=%g, vector_rst=%g\n", a[i], golden[i], vector_rst[i]);
    }
    printf("\n");
  }

  return 0;
}
