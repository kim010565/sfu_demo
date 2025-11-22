cmake_minimum_required(VERSION 3.10)
set(name_project "main")

project(main LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSC_DISABLE_API_VERSION_CHECK")
file(GLOB mainfile "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(${name_project} ${mainfile})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pg -march=native -mtune=native -fpermissive") 

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${name_project} stdc++fs)
