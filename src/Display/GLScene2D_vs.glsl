R"(#version 140

in vec3 vertex_position;
in vec2 tex_coords;

out vec2 texture_coordinates;

void main() {
	gl_Position = vec4(vertex_position, 1.0);
	texture_coordinates = tex_coords;
}
)"
