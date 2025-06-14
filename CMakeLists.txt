cmake_minimum_required(VERSION 3.5)

project(CG)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src/tinyxml)
add_subdirectory(src/model)
add_subdirectory(src/engine)
add_subdirectory(src/generator)

set_target_properties(engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set_target_properties(generator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)