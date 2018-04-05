#version 430

in vec2 texture_coordinates;

uniform sampler2D basic_texture;
uniform sampler2D basic_texture1;

out vec4 fragColour;

void main() {
	vec4 texel = texture (basic_texture, texture_coordinates);

	fragColour = texel;
}