attribute vec3 vertexPosition;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;

varying vec3 UV;

void main() {
	gl_Position = projMatrix * viewMatrix * vec4(vertexPosition, 1.0);
	UV = vertexPosition;
}