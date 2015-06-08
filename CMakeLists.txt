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

# Main executable
set(CF4H_SRC F4Helen/AST.cpp F4Helen/CGenTreeWalker.cpp F4Helen/LLVMTreeWalker.cpp F4Helen/TreeWalker.cpp)
set(CF4H_HEAD F4Helen/AST.h F4Helen/CGenTreeWalker.h F4Helen/LLVMTreeWalker.h F4Helen/TreeWalker.h)

add_executable(main ${CF4H_SRC})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs analysis core executionengine instcombine mcjit runtimedyld scalaropts
                                          support transformutils native)

# Link against LLVM libraries
target_link_libraries(main ${llvm_libs})

