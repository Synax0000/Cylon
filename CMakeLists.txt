cmake_minimum_required(VERSION 3.10)
project(Cylon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/Release)

file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.h"
)

add_executable(Cylon ${SourceFiles})

file(GLOB_RECURSE AssetFiles "${CMAKE_SOURCE_DIR}/resources/*")

add_custom_command(TARGET Cylon POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/resources"
    "$<TARGET_FILE_DIR:Cylon>/resources"
  COMMENT "Copying resources to bin directory"
)
