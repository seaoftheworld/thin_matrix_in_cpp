// Input positions are fixed values: 4 vertices with normalized x,y values from -1 ~ 1,
// this ensures the calculation for the uv of each vertex is correct
// Translate (not rotation now), scale is archieve by the model-matrix
attribute vec2 position;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

varying vec4 clipSpace;
varying vec2 uv;

// Specular lighting
const vec3 lightPosition = vec3(12.5, 12.5, 5.0);
varying vec3 vertexToCamera;
varying vec3 vertexToLight;

void main() {
	vec4 worldPosition = modelMatrix * vec4(position.x, position.y, 0.0, 1.0);
	gl_Position = projMatrix * viewMatrix * worldPosition;

	clipSpace = gl_Position;

	// Requires normalized values with x,y from -1 ~ 1 for the calculation below
	uv = vec2(position.x / 2.0 + 0.5, position.y / 2.0 + 0.5);

	vertexToCamera = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
	vertexToLight = worldPosition.xyz - lightPosition;
}
