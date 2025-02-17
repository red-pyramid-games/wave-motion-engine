CMAKE_MINIMUM_REQUIRED(VERSION 3.22.1)
project(wave-motion-engine)

set(CMAKE_C_STANDARD 17)
set(cimgui external/submodule/cimgui)
set(imgui_impl ${cimgui}/imgui/examples)

find_package(glfw3 REQUIRED)
find_package(cglm REQUIRED)
find_package(Freetype REQUIRED)

include_directories(${cimgui} ${cimgui}/imgui ${imgui_impl})
file(GLOB 
	cimgui_files 
	"${cimgui}/*.cpp" 
	"${cimgui}/imgui/*.cpp"
	"${imgui_impl}/imgui_impl_glfw.cpp"
	"${imgui_impl}/imgui_impl_opengl3.cpp")
add_library(cimgui ${cimgui_files})

add_executable(wave-motion-engine 
    src/editor.c
    src/main.c
    src/glad.c
    src/shader.c
    src/text.c
    src/texture.c
)

target_link_libraries(wave-motion-engine ${FREETYPE_LIBRARIES} glfw cglm cimgui)

target_include_directories(wave-motion-engine PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:inc)
target_include_directories(wave-motion-engine PRIVATE ${FREETYPE_INCLUDE_DIRS})
