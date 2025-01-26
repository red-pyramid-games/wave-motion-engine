# wave-motion-engine

I used clangd to compile this program and use lsp-zero for managing languages</br>
you may need to add either a .clangd file to the root of this project or </br>
a compile_flags.json. If you choose the .clangd you should add:</br>

CompileFlags:</br>
  Add: -I/home/username/path_if_not_in_home_dir/wave-motion-engine/inc</br>

Otherwise add this to your cmake file:</br>
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

libraries to install:
glfw3, cglm, libfreetype6 libfreetype6-dev freetype2-demos
