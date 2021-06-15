# BSD 3-Clause License; see https://github.com/scikit-hep/awkward-1.0/blob/main/LICENSE

cmake_minimum_required(VERSION 3.13...3.16)
project(example LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set up pybind11
add_subdirectory(pybind11 cmake_pybind11)

# set up Arrow
execute_process(
  COMMAND python -c "import pyarrow as pa; pa.create_library_symlinks()"
)
execute_process(
  COMMAND python -c "import pyarrow as pa; print(pa.get_include())"
  OUTPUT_VARIABLE PYARROW_INCLUDE
  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(
  COMMAND python -c "import pyarrow as pa; print(*pa.get_libraries(), sep=';')"
  OUTPUT_VARIABLE PYARROW_LIBRARIES
  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(
  COMMAND python -c "import pyarrow as pa; print(*pa.get_library_dirs(), sep=';')"
  OUTPUT_VARIABLE PYARROW_LIBRARY_DIRS
  OUTPUT_STRIP_TRAILING_WHITESPACE)

message(STATUS "PyArrow includes : ${PYARROW_INCLUDE}")
message(STATUS "PyArrow libraries : ${PYARROW_LIBRARIES}")
message(STATUS "PyArrow library dirs : ${PYARROW_LIBRARY_DIRS}")

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

find_library(CPU-KERNELS awkward-cpu-kernels REQUIRED HINTS ${AWKWARD_LIBRARIES})
find_library(LIBAWKWARD awkward REQUIRED HINTS ${AWKWARD_LIBRARIES})
find_library(LIBDL dl REQUIRED)
find_library(LIBARROW arrow REQUIRED HINTS ${PYARROW_LIBRARY_DIRS} NO_DEFAULT_PATH)
find_library(LIBARROW_PYTHON arrow_python REQUIRED HINTS ${PYARROW_LIBRARY_DIRS} NO_DEFAULT_PATH)
message(STATUS "Libraries: ${CPU-KERNELS} ${LIBAWKWARD} ${LIBDL} ${LIBARROW} ${LIBARROW_PYTHON}")

add_library(arrow IMPORTED INTERFACE)
target_link_libraries(arrow INTERFACE ${LIBARROW})
target_include_directories(arrow INTERFACE "${PYARROW_INCLUDE}")

add_library(pyarrow IMPORTED INTERFACE)
target_link_libraries(pyarrow INTERFACE arrow ${LIBARROW_PYTHON})
target_include_directories(pyarrow INTERFACE "${PYARROW_INCLUDE}")

add_library(awkward IMPORTED INTERFACE)
target_link_libraries(awkward INTERFACE ${CPU-KERNELS} ${LIBAWKWARD} ${LIBDL})
target_include_directories(awkward INTERFACE "${AWKWARD_INCLUDE}")

# minimal awkward+pybind11
pybind11_add_module(example example/python.cpp example/minimal.cpp)
set_target_properties(example PROPERTIES CXX_VISIBILITY_PRESET hidden)
target_link_libraries(example PRIVATE awkward)

# minimal arrow
add_executable(example_arrow example/arrow.cpp)
target_link_libraries(example_arrow PRIVATE arrow)

# awkward+pybind11+arrow
pybind11_add_module(babel babel/python.cpp)
set_target_properties(babel PROPERTIES CXX_VISIBILITY_PRESET hidden)
target_link_libraries(babel PRIVATE pyarrow awkward)
