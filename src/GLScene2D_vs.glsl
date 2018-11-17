#version 430

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 tex_coords;

out vec2 texture_coordinates;

void main() {
	gl_Position = vec4(vertex_position, 1.0);
	texture_coordinates = tex_coords;
}