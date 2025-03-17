CMAKE_MINIMUM_REQUIRED(VERSION 3.22.1)
project(wave-motion-engine)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)
find_package(cglm REQUIRED)
find_package(Freetype REQUIRED)
find_package(assimp REQUIRED)

add_executable(wave-motion-engine 
    src/buffer.c
    src/camera.c
    src/cube.c
    src/editor.c
    src/glad.c
    src/graphics.c
    src/keyboard.c
    src/light.c
    src/main.c
    src/material.c
    src/mesh.c
    src/model.c
    src/mouse.c
    src/point_light.c
    src/shader.c
    src/text.c
    src/texture.c
    src/time.c
    src/transform.c
    src/vertex.c
)

target_link_libraries(wave-motion-engine glfw)
target_link_libraries(wave-motion-engine cglm)
target_link_libraries(wave-motion-engine ${FREETYPE_LIBRARIES})
target_link_libraries(wave-motion-engine assimp)

target_include_directories(wave-motion-engine PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:inc)
target_include_directories(wave-motion-engine PRIVATE ${FREETYPE_INCLUDE_DIRS})
target_include_directories(wave-motion-engine PRIVATE ${ASSIMP_INCLUDE_DIRS})
