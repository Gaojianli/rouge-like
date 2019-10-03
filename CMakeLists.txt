# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("rouge-like")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++17")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2")
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")
AUX_SOURCE_DIRECTORY(src DIR_SRCS)
add_executable(rouge-like
   ${DIR_SRCS}
)
target_link_libraries(rouge-like "${PROJECT_SOURCE_DIR}/pdcurses.lib")
