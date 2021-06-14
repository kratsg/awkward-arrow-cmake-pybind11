# BSD 3-Clause License; see https://github.com/scikit-hep/awkward-1.0/blob/main/LICENSE

cmake_minimum_required(VERSION 3.1...3.16)
project(example LANGUAGES CXX)

execute_process(
  COMMAND python3 -m awkward.config --incdir
  OUTPUT_VARIABLE AWKWARD_INCLUDE
  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(
  COMMAND python3 -m awkward.config --libdir
  OUTPUT_VARIABLE AWKWARD_LIBRARIES
  OUTPUT_STRIP_TRAILING_WHITESPACE)

include_directories(BEFORE "${AWKWARD_INCLUDE}")

add_subdirectory(pybind11 cmake_pybind11)

include(FetchContent REQUIRED)
FetchContent_Declare(
        arrow
        GIT_REPOSITORY https://github.com/apache/arrow.git
        GIT_TAG        apache-arrow-0.15.1
)
FetchContent_GetProperties(arrow)
FetchContent_MakeAvailable(arrow)

message(STATUS "Arrow version: ${ARROW_VERSION}")
message(STATUS "Arrow SO version: ${ARROW_FULL_SO_VERSION}")

find_library(CPU-KERNELS awkward-cpu-kernels REQUIRED HINTS ${AWKWARD_LIBRARIES})
find_library(LIBAWKWARD awkward REQUIRED HINTS ${AWKWARD_LIBRARIES})
find_library(LIBDL dl REQUIRED)
message(STATUS "Libraries: ${CPU-KERNELS} ${LIBAWKWARD} ${LIBDL}")

# awkward+pybind11
pybind11_add_module(example example/python.cpp example/minimal.cpp)
# minimal arrow
add_executable(example example/arrow.cpp)
set_target_properties(example PROPERTIES CXX_VISIBILITY_PRESET hidden)
# statically-link arrow
target_link_libraries(example PRIVATE ${CPU-KERNELS} ${LIBAWKWARD} ${LIBDL} PRIVATE arrow_static Threads::Threads)
# shared-link arrow
# target_link_libraries(example PRIVATE arrow_shared)
