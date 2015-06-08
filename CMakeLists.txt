# The project tile
cmake_minimum_required(VERSION 2.8)
project(CF4H)

# Versioning
set (CF4H_VERSION_MAJOR 1)
set (CF4H_VERSION_MINOR 0)

# LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Flex/Bison
find_package(BISON)
find_package(FLEX)
BISON_TARGET(cf4parser Grammar/cf4.y ${PROJECT_SOURCE_DIR}/Grammar/cf4.y.cpp)
FLEX_TARGET(cf4scanner Grammar/cf4.l ${PROJECT_SOURCE_DIR}/Grammar/cf4.l.cpp)
ADD_FLEX_BISON_DEPENDENCY(cf4scanner cf4parser)

# Main executable
set(CF4H_SRC main.cpp F4Helen/AST.cpp F4Helen/CGenTreeWalker.cpp F4Helen/LLVMTreeWalker.cpp F4Helen/TreeWalker.cpp)
set(CF4H_HEAD version.h F4Helen/AST.h F4Helen/CGenTreeWalker.h F4Helen/LLVMTreeWalker.h F4Helen/TreeWalker.h)

add_executable(cf4h ${CF4H_SRC} ${BISON_cf4parser_OUTPUTS} ${FLEX_cf4scanner_OUTPUTS})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs core)

# message(FATAL_ERROR "LLVM libs: ${llvm_libs}")

# Link against LLVM libraries
target_link_libraries(cf4h ${llvm_libs})

