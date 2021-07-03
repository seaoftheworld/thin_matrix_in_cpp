// Input positions are fixed values: 4 vertices with normalized x,y values from -1 ~ 1,
// this ensures the calculation for the uv of each vertex is correct
// Translate (no rotation now), scale are from model-matrix
attribute vec2 position;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

// varying vec4 clipSpace;
varying vec2 uv;

// Specular Lighting
// const vec3 lightPosition = vec3(0.0, 0.0, 500.0);  // unable to be seamless when multi water-tile connects if light is too close
// const vec3 lightPosition = vec3(500.0, 500.0, 500.0);  // unable to be seamless when multi water-tile connects if light is too close
// const vec3 lightPosition = vec3(50.0, 50.0, 500.0);  // unable to be seamless when multi water-tile connects if light is too close
const vec3 lightPosition00 = vec3( 2.5,  2.5, 500.0);  // unable to be seamless when multi water-tile connects if light is too close
const vec3 lightPosition01 = vec3( 2.5, -2.5, 500.0);  // unable to be seamless when multi water-tile connects if light is too close
const vec3 lightPosition02 = vec3(-2.5,  2.5, 500.0);  // unable to be seamless when multi water-tile connects if light is too close
const vec3 lightPosition03 = vec3(-2.5, -2.5, 500.0);  // unable to be seamless when multi water-tile connects if light is too close


// uniform vec3 lightPosition;
varying vec3 vertexToCamera;
varying vec3 vertexToLight00;
varying vec3 vertexToLight01;
varying vec3 vertexToLight02;
varying vec3 vertexToLight03;

void main() {
	// vec4 vertexWorldPosition = modelMatrix * vec4(position.x, position.y, 0.0, 0.0);
	vec4 vertexWorldPosition = modelMatrix * vec4(position.x, position.y, 0.0, 1.0);
	vec4 lightWorldPosition00 = modelMatrix * vec4(lightPosition00, 1.0);
	vec4 lightWorldPosition01 = modelMatrix * vec4(lightPosition01, 1.0);
	vec4 lightWorldPosition02 = modelMatrix * vec4(lightPosition02, 1.0);
	vec4 lightWorldPosition03 = modelMatrix * vec4(lightPosition03, 1.0);

	gl_Position = projMatrix * viewMatrix * vertexWorldPosition;
	// clipSpace = gl_Position;

	// Requires normalized values with x,y from -1 ~ 1 for the calculation below
	uv = vec2(position.x / 2.0 + 0.5, position.y / 2.0 + 0.5);

	vertexToCamera = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - vertexWorldPosition.xyz;
	// vertexToLight = vertexWorldPosition.xyz - lightWorldPosition.xyz;
	// vertexToLight = vertexWorldPosition.xyz - lightPosition;

	vertexToLight00 = vertexWorldPosition.xyz - lightWorldPosition00.xyz;
	vertexToLight01 = vertexWorldPosition.xyz - lightWorldPosition01.xyz;
	vertexToLight02 = vertexWorldPosition.xyz - lightWorldPosition02.xyz;
	vertexToLight03 = vertexWorldPosition.xyz - lightWorldPosition03.xyz;
}
