import numpy as np
from ctypes import c_float, c_uint32


def float_to_uint32(f):
    cf = c_float(f)
    cu = c_uint32.from_buffer_copy(cf)
    return np.uint32(cu.value & 0xFFFFFFFF)


def uint32to_float(u):
    cu = c_uint32(u)
    cf = c_float.from_buffer_copy(cu)
    return cf.value


with open("sfu_table.txt", "w") as f:
    # exp2
    offset = np.float32(
        1.0
    )  # 使得偏移后，start_v和end_v的指数不分相同，方便仅用尾数查表
    start_x = np.float32(offset + 0.0)
    end_x = np.float32(offset + 0.9999999)

    start_x_u32 = float_to_uint32(start_x)  # 将浮点数转化为32位无符号整数
    end_x_u32 = float_to_uint32(end_x)
    x_exp = start_x_u32 >> 23  # 取指数
    start_x_fraction = start_x_u32 & 0x7FFFFF  # 取尾数
    end_x_fraction = end_x_u32 & 0x7FFFFF

    m = 6
    start_x1 = start_x_fraction >> (23 - m)  # 取尾数的前m位
    end_x1 = end_x_fraction >> (23 - m)

    extra = 10
    x2 = np.arange(
        -extra + 0, 2 ** (23 - m) + extra
    )  # 遍历x2所有的可能取值, extra是为了防止拟合边缘异常的额外值

    f.write("constexpr uint32_t exp2_table[][3] = {\n")
    for x1 in range(start_x1, end_x1 + 1):  # 遍历x1所有的可能取值
        x_u32 = (x_exp << 23) + (x1 << (23 - m)) + x2  # 将x1和x2组合成32位无符号整数
        x = [uint32to_float(tmp) for tmp in x_u32]  # 将32位无符号整数转化为浮点数
        x_double = np.double(x)
        y_double = np.exp2(x_double - offset)
        coef_double = np.polyfit(x_double, y_double, 2)  # 拟合二次多项式
        coef = np.float32(coef_double)
        coef_u32 = [float_to_uint32(tmp) for tmp in coef]
        print(
            "    {0x%x, 0x%x, 0x%x}," % (coef_u32[0], coef_u32[1], coef_u32[2]),
            file=f,
        )
    f.write("};\n")

    # log
    start_x = np.float32(0.5)
    end_x = np.float32(0.9999999)

    start_x_u32 = float_to_uint32(start_x)  # 将浮点数转化为32位无符号整数
    end_x_u32 = float_to_uint32(end_x)
    x_exp = start_x_u32 >> 23  # 取指数
    start_x_fraction = start_x_u32 & 0x7FFFFF  # 取尾数
    end_x_fraction = end_x_u32 & 0x7FFFFF

    m = 6
    start_x1 = start_x_fraction >> (23 - m)  # 取尾数的前m位
    end_x1 = end_x_fraction >> (23 - m)

    extra = 10
    x2 = np.arange(
        -extra + 0, 2 ** (23 - m) + extra
    )  # 遍历x2所有的可能取值, extra是为了防止拟合边缘异常的额外值

    f.write("constexpr uint32_t log_table[][3] = {\n")
    for x1 in range(start_x1, end_x1 + 1):  # 遍历x1所有的可能取值
        x_u32 = (x_exp << 23) + (x1 << (23 - m)) + x2  # 将x1和x2组合成32位无符号整数
        x = [uint32to_float(tmp) for tmp in x_u32]  # 将32位无符号整数转化为浮点数
        x_double = np.double(x)
        y_double = np.log(x_double)
        coef_double = np.polyfit(x_double, y_double, 2)  # 拟合二次多项式
        coef = np.float32(coef_double)
        coef_u32 = [float_to_uint32(tmp) for tmp in coef]
        print(
            "    {0x%x, 0x%x, 0x%x}," % (coef_u32[0], coef_u32[1], coef_u32[2]),
            file=f,
        )
    f.write("};\n")

    # sin
    offset = np.float32(
        1.0
    )  # 使得偏移后，start_v和end_v的指数不分相同，方便仅用尾数查表
    start_x = np.float32(offset + 0.0)
    end_x = np.float32(offset + 0.7853981633974483096)

    start_x_u32 = float_to_uint32(start_x)  # 将浮点数转化为32位无符号整数
    end_x_u32 = float_to_uint32(end_x)
    x_exp = start_x_u32 >> 23  # 取指数
    start_x_fraction = start_x_u32 & 0x7FFFFF  # 取尾数
    end_x_fraction = end_x_u32 & 0x7FFFFF

    m = 6
    start_x1 = start_x_fraction >> (23 - m)  # 取尾数的前m位
    end_x1 = end_x_fraction >> (23 - m)

    extra = 10
    x2 = np.arange(
        -extra + 0, 2 ** (23 - m) + extra
    )  # 遍历x2所有的可能取值, extra是为了防止拟合边缘异常的额外值

    f.write("constexpr uint32_t sin_table[][3] = {\n")
    for x1 in range(start_x1, end_x1 + 1):  # 遍历x1所有的可能取值
        x_u32 = (x_exp << 23) + (x1 << (23 - m)) + x2  # 将x1和x2组合成32位无符号整数
        x = [uint32to_float(tmp) for tmp in x_u32]  # 将32位无符号整数转化为浮点数
        x_double = np.double(x)
        y_double = np.sin(x_double - offset)
        coef_double = np.polyfit(x_double, y_double, 2)  # 拟合二次多项式
        coef = np.float32(coef_double)
        coef_u32 = [float_to_uint32(tmp) for tmp in coef]
        print(
            "    {0x%x, 0x%x, 0x%x}," % (coef_u32[0], coef_u32[1], coef_u32[2]),
            file=f,
        )
    f.write("};\n")

    # cos
    offset = np.float32(
        1.0
    )  # 使得偏移后，start_v和end_v的指数不分相同，方便仅用尾数查表
    start_x = np.float32(offset + 0.0)
    end_x = np.float32(offset + 0.7853981633974483096)

    start_x_u32 = float_to_uint32(start_x)  # 将浮点数转化为32位无符号整数
    end_x_u32 = float_to_uint32(end_x)
    x_exp = start_x_u32 >> 23  # 取指数
    start_x_fraction = start_x_u32 & 0x7FFFFF  # 取尾数
    end_x_fraction = end_x_u32 & 0x7FFFFF

    m = 6
    start_x1 = start_x_fraction >> (23 - m)  # 取尾数的前m位
    end_x1 = end_x_fraction >> (23 - m)

    extra = 10
    x2 = np.arange(
        -extra + 0, 2 ** (23 - m) + extra
    )  # 遍历x2所有的可能取值, extra是为了防止拟合边缘异常的额外值

    f.write("constexpr uint32_t cos_table[][3] = {\n")
    for x1 in range(start_x1, end_x1 + 1):  # 遍历x1所有的可能取值
        x_u32 = (x_exp << 23) + (x1 << (23 - m)) + x2  # 将x1和x2组合成32位无符号整数
        x = [uint32to_float(tmp) for tmp in x_u32]  # 将32位无符号整数转化为浮点数
        x_double = np.double(x)
        y_double = np.cos(x_double - offset)
        coef_double = np.polyfit(x_double, y_double, 2)  # 拟合二次多项式
        coef = np.float32(coef_double)
        coef_u32 = [float_to_uint32(tmp) for tmp in coef]
        print(
            "    {0x%x, 0x%x, 0x%x}," % (coef_u32[0], coef_u32[1], coef_u32[2]),
            file=f,
        )
    f.write("};\n")

    # rcp
    start_x = np.float32(0.5)
    end_x = np.float32(0.9999999)

    start_x_u32 = float_to_uint32(start_x)  # 将浮点数转化为32位无符号整数
    end_x_u32 = float_to_uint32(end_x)
    x_exp = start_x_u32 >> 23  # 取指数
    start_x_fraction = start_x_u32 & 0x7FFFFF  # 取尾数
    end_x_fraction = end_x_u32 & 0x7FFFFF

    m = 6
    start_x1 = start_x_fraction >> (23 - m)  # 取尾数的前m位
    end_x1 = end_x_fraction >> (23 - m)

    extra = 10
    x2 = np.arange(
        -extra + 0, 2 ** (23 - m) + extra
    )  # 遍历x2所有的可能取值, extra是为了防止拟合边缘异常的额外值

    f.write("constexpr uint32_t rcp_table[][3] = {\n")
    for x1 in range(start_x1, end_x1 + 1):  # 遍历x1所有的可能取值
        x_u32 = (x_exp << 23) + (x1 << (23 - m)) + x2  # 将x1和x2组合成32位无符号整数
        x = [uint32to_float(tmp) for tmp in x_u32]  # 将32位无符号整数转化为浮点数
        x_double = np.double(x)
        y_double = np.double(1.0) / x_double
        coef_double = np.polyfit(x_double, y_double, 2)  # 拟合二次多项式
        coef = np.float32(coef_double)
        coef_u32 = [float_to_uint32(tmp) for tmp in coef]
        print(
            "    {0x%x, 0x%x, 0x%x}," % (coef_u32[0], coef_u32[1], coef_u32[2]),
            file=f,
        )
    f.write("};\n")

