# BSD 3-Clause License; see https://github.com/scikit-hep/awkward-1.0/blob/main/LICENSE

cmake_minimum_required(VERSION 3.1...3.16)
project(example LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set up pybind11
add_subdirectory(pybind11 cmake_pybind11)

# set up Arrow
find_package(Arrow REQUIRED)
execute_process(
  COMMAND python -c "import pyarrow as pa; print(pa.get_include())"
  OUTPUT_VARIABLE PYARROW_INCLUDE
  OUTPUT_STRIP_TRAILING_WHITESPACE)

message(STATUS "Arrow version: ${ARROW_VERSION}")
message(STATUS "Arrow SO version: ${ARROW_FULL_SO_VERSION}")
message(STATUS "PyArrow includes : ${PYARROW_INCLUDE}")
include_directories(BEFORE "${PYARROW_INCLUDE}")

# set up awkward
execute_process(
  COMMAND python -m awkward.config --incdir
  OUTPUT_VARIABLE AWKWARD_INCLUDE
  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(
  COMMAND python -m awkward.config --libdir
  OUTPUT_VARIABLE AWKWARD_LIBRARIES
  OUTPUT_STRIP_TRAILING_WHITESPACE)

message(STATUS "Awkward includes : ${AWKWARD_INCLUDE}")
message(STATUS "Awkward libraries: ${AWKWARD_LIBRARIES}")
include_directories(BEFORE "${AWKWARD_INCLUDE}")

find_library(CPU-KERNELS awkward-cpu-kernels REQUIRED HINTS ${AWKWARD_LIBRARIES})
find_library(LIBAWKWARD awkward REQUIRED HINTS ${AWKWARD_LIBRARIES})
find_library(LIBDL dl REQUIRED)
message(STATUS "Libraries: ${CPU-KERNELS} ${LIBAWKWARD} ${LIBDL}")

# minimal awkward+pybind11
pybind11_add_module(example example/python.cpp example/minimal.cpp)
set_target_properties(example PROPERTIES CXX_VISIBILITY_PRESET hidden)
target_link_libraries(example PRIVATE ${CPU-KERNELS} ${LIBAWKWARD} ${LIBDL})

# minimal arrow
add_executable(example_arrow example/arrow.cpp)
target_link_libraries(example_arrow PRIVATE arrow_shared)

# awkward+pybind11+arrow
pybind11_add_module(babel babel/python.cpp)
set_target_properties(babel PROPERTIES CXX_VISIBILITY_PRESET hidden)
target_link_libraries(babel PRIVATE ${PYARROW_LIBRARIES})
