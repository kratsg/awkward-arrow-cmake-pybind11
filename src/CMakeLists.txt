# BSD 3-Clause License; see https://github.com/scikit-hep/awkward-1.0/blob/main/LICENSE

cmake_minimum_required(VERSION 3.1...3.16)
project(example LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

execute_process(
  COMMAND python3 -m awkward.config --incdir
  OUTPUT_VARIABLE AWKWARD_INCLUDE
  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(
  COMMAND python3 -m awkward.config --libdir
  OUTPUT_VARIABLE AWKWARD_LIBRARIES
  OUTPUT_STRIP_TRAILING_WHITESPACE)

message(STATUS "Awkward includes : ${AWKWARD_INCLUDE}")
message(STATUS "Awkward libraries: ${AWKWARD_LIBRARIES}")
include_directories(BEFORE "${AWKWARD_INCLUDE}")

add_subdirectory(pybind11 cmake_pybind11)
find_package(Arrow REQUIRED)

message(STATUS "Arrow version: ${ARROW_VERSION}")
message(STATUS "Arrow SO version: ${ARROW_FULL_SO_VERSION}")

find_library(CPU-KERNELS awkward-cpu-kernels REQUIRED HINTS ${AWKWARD_LIBRARIES})
find_library(LIBAWKWARD awkward REQUIRED HINTS ${AWKWARD_LIBRARIES})
find_library(LIBDL dl REQUIRED)
message(STATUS "Libraries: ${CPU-KERNELS} ${LIBAWKWARD} ${LIBDL}")

# awkward+pybind11
pybind11_add_module(example_awkward example/python.cpp example/minimal.cpp)
# minimal arrow
add_executable(example_arrow example/arrow.cpp)
set_target_properties(example_awkward PROPERTIES CXX_VISIBILITY_PRESET hidden)
target_link_libraries(example_awkward PRIVATE ${CPU-KERNELS} ${LIBAWKWARD} ${LIBDL})

# statically-link arrow
# target_link_libraries(example_arrow PRIVATE arrow_static Threads::Threads)
# shared-link arrow
target_link_libraries(example_arrow PRIVATE arrow_shared)
