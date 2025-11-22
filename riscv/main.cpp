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

    vfloat32m4_t x = __riscv_vle32_v_f32m4(a, 16);
    vfloat32m4_t y = expf(x, 16);
    __riscv_vse32_v_f32m4(vector_rst, y, 16);

    for (int i = 0; i < 16; i++) {
      printf("riscv expf test: input=%g, golden=%g, vector_rst=%g\n", a[i], golden[i], vector_rst[i]);
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

    vfloat32m4_t x = __riscv_vle32_v_f32m4(a, 16);
    vfloat32m4_t y = logf(x, 16);
    __riscv_vse32_v_f32m4(vector_rst, y, 16);

    for (int i = 0; i < 16; i++) {
      printf("riscv logf test: input=%g, golden=%g, vector_rst=%g\n", a[i], golden[i], vector_rst[i]);
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

    vfloat32m4_t x = __riscv_vle32_v_f32m4(a, 16);
    vfloat32m4_t y = sinf(x, 16);
    __riscv_vse32_v_f32m4(vector_rst, y, 16);

    for (int i = 0; i < 16; i++) {
      printf("riscv sinf test: input=%g, golden=%g, vector_rst=%g\n", a[i], golden[i], vector_rst[i]);
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

    vfloat32m4_t x = __riscv_vle32_v_f32m4(a, 16);
    vfloat32m4_t y = cosf(x, 16);
    __riscv_vse32_v_f32m4(vector_rst, y, 16);

    for (int i = 0; i < 16; i++) {
      printf("riscv cosf test: input=%g, golden=%g, vector_rst=%g\n", a[i], golden[i], vector_rst[i]);
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

    vfloat32m4_t x = __riscv_vle32_v_f32m4(a, 16);
    vfloat32m4_t y = __riscv_vfrec7_v_f32m4(x, 16);                                                          // 近似值: y = 1/x
    y = __riscv_vfmul_vv_f32m4(__riscv_vfnmsac_vv_f32m4(__riscv_vfmv_v_f_f32m4(2.0, 16), x, y, 16), y, 16);  // 第一次牛顿迭代：y = (2.0 - x * y) * y
    y = __riscv_vfmul_vv_f32m4(__riscv_vfnmsac_vv_f32m4(__riscv_vfmv_v_f_f32m4(2.0, 16), x, y, 16), y, 16);  // 第二次牛顿迭代
    __riscv_vse32_v_f32m4(vector_rst, y, 16);

    for (int i = 0; i < 16; i++) {
      printf("riscv rcp test: input=%g, golden=%g, vector_rst=%g\n", a[i], golden[i], vector_rst[i]);
    }
    printf("\n");
  }

  return 0;
}
