cmake_minimum_required(VERSION 3.10)
set(name_project "main")

project(main LANGUAGES C CXX)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER aarch64-none-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-none-linux-gnu-g++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSC_DISABLE_API_VERSION_CHECK")
file(GLOB mainfile "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(${name_project} ${mainfile})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pg -march=armv8-a+crc+crypto -mtune=cortex-a53") 

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${name_project} stdc++fs)
