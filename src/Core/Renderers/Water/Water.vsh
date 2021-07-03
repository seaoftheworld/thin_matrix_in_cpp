// Input positions are fixed values: 4 vertices with normalized x,y values from -1 ~ 1,
// this ensures the calculation for the uv of this shader is correct
// Transform is archieve by the model-matrix
attribute vec2 position;

varying vec2 uv;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main() {
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position.x, position.y, 0.0, 1.0);

	// Requires normalized values with x,y from -1 ~ 1 for the calculation below
	uv = vec2(position.x / 2.0 + 0.5, position.y / 2.0 + 0.5);
}
