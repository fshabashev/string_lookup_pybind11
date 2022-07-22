cmake_minimum_required(VERSION 3.22)
project(cpp_clion)

set(CMAKE_CXX_STANDARD 14)

add_executable(cpp_clion main.cpp)


include_directories(pybind11/include)

add_subdirectory(pybind11)

# find_package(PythonLibs REQUIRED)

find_package(Python COMPONENTS Interpreter Development)

execute_process(COMMAND python3 -c "import sysconfig; print(sysconfig.get_path('include'))" OUTPUT_VARIABLE PYTHON_INCLUDE_DIRS)

message(PYTHON_INCLUDE_DIRS ${PYTHON_INCLUDE_DIRS})

include_directories(${PYTHON_INCLUDE_DIRS})

message("${PYTHON_INCLUDE_DIRS}")
message("${PYTHON_LIBRARIES}")
message("${PYTHON_VERSION}")

pybind11_add_module(cpp_string_lookup main.cpp)
target_link_libraries(cpp_clion ${PYTHON_LIBRARIES})