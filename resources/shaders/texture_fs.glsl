#version 330 core
out vec4 frag_color;

in vec3 out_color;
in vec2 out_tex_coord;

uniform sampler2D texture1;

void main() {
    frag_color = texture(texture1, out_tex_coord);
}
