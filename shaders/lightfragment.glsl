#version 330 core

out vec4 FragColor;
uniform vec3 lightboxcolor;

void main() {
	FragColor = vec4(lightboxcolor, 1.0);
}