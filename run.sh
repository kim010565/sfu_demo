#!/bin/bash

# 当前脚本路径
SCRIPT_PATH=$(dirname "$(readlink -f "$0")")

# x86
cd ${SCRIPT_PATH}/x86
rm -rf build

mkdir -p build
cd build
cmake ..
make -j8

./main

cd ..

# risc-v
cd ${SCRIPT_PATH}/riscv

if [ ! -e Xuantie-qemu-x86_64-Ubuntu-20.04-V5.2.8-B20250721-0303.tar.gz ]; then
    wget https://occ-oss-prod.oss-cn-hangzhou.aliyuncs.com/resource//1753237318263/Xuantie-qemu-x86_64-Ubuntu-20.04-V5.2.8-B20250721-0303.tar.gz
    tar -zxvf Xuantie-qemu-x86_64-Ubuntu-20.04-V5.2.8-B20250721-0303.tar.gz
    sed -i 's/\r$//' ${SCRIPT_PATH}/riscv/install/qemu_post_install_small.sh
    sed -i 's/\r/\n/g' ${SCRIPT_PATH}/riscv/install/qemu_post_install_small.sh
    bash ${SCRIPT_PATH}/riscv/install/qemu_post_install_small.sh
    apt-get install -y libcapstone4
    ln -s /usr/lib/x86_64-linux-gnu/libcapstone.so.4 /usr/lib/x86_64-linux-gnu/libcapstone.so.3
fi

if [ ! -e Xuantie-900-gcc-linux-6.6.0-glibc-x86_64-V3.2.0-20250627.tar.gz ]; then
    wget https://occ-oss-prod.oss-cn-hangzhou.aliyuncs.com/resource//1755679709552/Xuantie-900-gcc-linux-6.6.0-glibc-x86_64-V3.2.0-20250627.tar.gz
    tar -zxvf Xuantie-900-gcc-linux-6.6.0-glibc-x86_64-V3.2.0-20250627.tar.gz
fi

export PATH=${SCRIPT_PATH}/riscv/Xuantie-900-gcc-linux-6.6.0-glibc-x86_64-V3.2.0/bin:${SCRIPT_PATH}/riscv/install/bin:$PATH

rm -rf build

mkdir -p build
cd build
cmake ..
make -j8

qemu-riscv64 -L ${SCRIPT_PATH}/riscv/Xuantie-900-gcc-linux-6.6.0-glibc-x86_64-V3.2.0/sysroot main

cd ..

# arm neon
cd ${SCRIPT_PATH}/arm

if [ ! -e arm-gnu-toolchain-14.3.rel1-x86_64-aarch64-none-linux-gnu.tar.xz ]; then
    wget https://developer.arm.com/-/media/Files/downloads/gnu/14.3.rel1/binrel/arm-gnu-toolchain-14.3.rel1-x86_64-aarch64-none-linux-gnu.tar.xz
    tar -xf arm-gnu-toolchain-14.3.rel1-x86_64-aarch64-none-linux-gnu.tar.xz
fi
apt-get install -y qemu-user-static
export PATH=${SCRIPT_PATH}/arm/arm-gnu-toolchain-14.3.rel1-x86_64-aarch64-none-linux-gnu/bin:$PATH

rm -rf build

mkdir -p build
cd build
cmake ..
make -j8

qemu-aarch64-static -L ${SCRIPT_PATH}/arm/arm-gnu-toolchain-14.3.rel1-x86_64-aarch64-none-linux-gnu/aarch64-none-linux-gnu/libc main

cd ..