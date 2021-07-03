#version 140  // 140 for function inverse()

// Inputs
in vec3 vertexPosition;
in vec2 vertexUV;
in vec3 vertexNormal;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformMatrix;

uniform vec3 lightPosition[8];

const vec4 plane = vec4(0, 0, -1, 1.5);
// uniform vec4 plane;

// To fragment shader
out vec2 uv;

out vec3 vertexToCamera;
out vec3 vertexSurface;
out vec3 vertexToLight[8];

void main()
{
    vec4 vertexWorldPosition = transformMatrix * vec4(vertexPosition, 1.0);
    gl_ClipDistance[0] = dot(vertexWorldPosition, plane);

    gl_Position = projMatrix * viewMatrix * vertexWorldPosition;

    vertexToCamera = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - vertexWorldPosition.xyz;
    vertexSurface = (transformMatrix * vec4(vertexNormal, 0.0)).xyz;
    uv = vertexUV;
    
    for (int i = 0; i < 8; i++) {
        vertexToLight[i] = lightPosition[i] - vertexWorldPosition.xyz;
    }
}
