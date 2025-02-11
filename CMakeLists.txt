CMAKE_MINIMUM_REQUIRED(VERSION 3.22.1)
project(wave-motion-engine)

set(CMAKE_C_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(cglm REQUIRED)
find_package(Freetype REQUIRED)
message(${FREETYPE_LIBRARIES})
message(${FREETYPE_INCLUDE_DIRS})

add_executable(wave-motion-engine 
    src/editor.c
    src/main.c
    src/glad.c
    src/shader.c
)

target_link_libraries(wave-motion-engine glfw)
target_link_libraries(wave-motion-engine cglm)
target_link_libraries(wave-motion-engine ${FREETYPE_LIBRARIES})

target_include_directories(wave-motion-engine PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:inc)
target_include_directories(wave-motion-engine PRIVATE ${FREETYPE_INCLUDE_DIRS})
