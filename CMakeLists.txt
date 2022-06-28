cmake_minimum_required(VERSION 3.21)
project(untitled2)

set(CMAKE_CXX_STANDARD 17)
set(SDL2_INCLUDE_DIR C:/SDL2/include)
set(SDL2_LIB_DIR C:/SDL2/lib/x64)

set(SDL2_INCLUDE_DIR1 C:/SDL2_ttf/include)
set(SDL2_LIB_DIR2 C:/SDL2_ttf/lib/x64)

set(SDL2_INCLUDE_DIR3 C:/SDL2_image/include)
set(SDL2_LIB_DIR4 C:/SDL2_image/lib/x64)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

include_directories(${SDL2_INCLUDE_DIR1})
link_directories(${SDL2_LIB_DIR2})

include_directories(${SDL2_INCLUDE_DIR3})
link_directories(${SDL2_LIB_DIR4})

add_executable(untitled2 main.cpp Menu_Obj.cpp Menu_Obj.h Game_Obj.cpp Game_Obj.h)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2)
target_link_libraries(${PROJECT_NAME} SDL2_ttf)
target_link_libraries(${PROJECT_NAME} SDL2_image)

