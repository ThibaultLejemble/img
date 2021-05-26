project(img)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS                "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG          "-g3 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE        "-DNDEBUG -O3")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3")

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE FILE_H   src/*.h  )
file(GLOB_RECURSE FILE_CPP src/*.cpp)
set(SOURCES ${FILE_H} ${FILE_CPP})

include_directories(src external/Eigen/)

add_executable(example0_test examples/example0_test.cpp)

add_executable(example1_fractal examples/example1_fractal.cpp ${SOURCES})
add_executable(example2_binary  examples/example2_binary.cpp  ${SOURCES})
