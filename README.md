# sfu_demo
sfu_demo

## 软件拟合方法
标量参考：https://www.netlib.org/cephes/singldoc.html
向量参考(基于cephes库，改成SIMD实现)：
1) https://gitlab.com/libeigen/eigen/-/blob/master/Eigen/src/Core/arch/Default/GenericPacketMathFunctions.h
2) https://github.com/Tencent/ncnn/blob/master/src/layer/x86/avx_mathfun.h
3) https://github.com/ARM-software/ComputeLibrary/blob/main/src/core/NEON/NEMath.inl (拟合可能与cephes略微不同，项数更少)

exp/log/sin/cos，步骤基本都是：
1) 缩小范围
2) 多项式拟合
3) 恢复范围

精度更低的exp拟合方法：https://blog.csdn.net/shakingWaves/article/details/78450500?locationNum=6&fps=1
牛顿迭代（比如用于求倒数）参考：https://zhuanlan.zhihu.com/p/476420407
CPU倒数指令参考：
https://github.com/ucb-bar/berkeley-softfloat-3/blob/master/source/s_approxRecip32_1.c
https://github.com/qemu/qemu/blob/e88510fcdc13380bd4895a17d6f8a0b3a3325b85/target/riscv/vector_helper.c#L3980

### x86软件拟合代码参考
1) [x86/expf.h](./x86/expf.h)
2) [x86/logf.h](./x86/logf.h)
3) [x86/sinf.h](./x86/sinf.h)

### riscv软件拟合代码参考
1) [riscv/expf.h](./riscv/expf.h)
2) [riscv/logf.h](./riscv/logf.h)
3) [riscv/sinf.h](./riscv/sinf.h)

### neon软件拟合代码参考
1) [arm/expf.h](./arm/expf.h)
2) [arm/logf.h](./arm/logf.h)
3) [arm/sinf.h](./arm/sinf.h)

## 硬件拟合方法
参考《High-Speed Function Approximation Using a Minimax Quadratic Interpolator》
思路基本与软件拟合方法相同，也是
1) 缩小范围
2) 多项式拟合
3) 恢复范围

区别是：
缩小范围后，取值范围内的指数部分相同。
根据尾数部分取高m bit查表（共2^m个表项），相当于根据高m bit做了分段拟合，每段的取值范围变成低(23-m)bit。
由于分段后的取值范围更小了，所以可以只用二次项拟合，而不是多项式拟合，就能得到比较高的精度。

一般硬件中为什么取exp2(x)，而不是exp(x)，参考：https://zhuanlan.zhihu.com/p/1892718058298114185

### 硬件拟合代码参考
1) [x86/sfu.h](./x86/sfu.h)
2) [x86/sfu_table.py](./x86/sfu_table.py) (用于产生拟合table)
