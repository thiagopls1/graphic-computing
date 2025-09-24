#version 330

in vec4 vCol;

uniform vec3 triangleColor;

out vec4 color;

void main() {
	color = vCol;
}