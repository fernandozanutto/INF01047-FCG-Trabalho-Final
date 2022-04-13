cmake_minimum_required(VERSION 3.0.0)
project(TrabFinal VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output)

add_library(Libs
    src/glad.c
    src/ObjModel.cpp
    src/stb_image.cpp
    src/textrendering.cpp
    src/tiny_obj_loader.cpp
)

add_executable(TrabFinal src/main.cpp)

target_include_directories(Libs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(TrabFinal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB libs_user lib/*.dll)
file(GLOB libs_user2 lib/*.a)

target_link_libraries(TrabFinal Libs ${libs_user} ${libs_user2} -lgdi32 -lopengl32)

add_custom_command(TARGET TrabFinal PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/src/data $<TARGET_FILE_DIR:TrabFinal>/data)

add_custom_command(TARGET TrabFinal PRE_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_directory
                           ${CMAKE_SOURCE_DIR}/src/shaders $<TARGET_FILE_DIR:TrabFinal>/shaders)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
