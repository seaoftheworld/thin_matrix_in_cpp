// Inputs
attribute vec3 vertexPosition;
attribute vec2 vertexUV;
attribute vec3 vertexNormal;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformMatrix;

uniform vec3 lightPosition;

// To fragment shader
varying vec2 uv;

varying vec3 vertexSurface;
varying vec3 vertexToLight;
varying vec3 vertexToCamera;

// uniform float tFloat;

void main()
{
    // gl_Position.xyz = vertexPosition;
    // gl_Position.x += tFloat;
    // gl_Position.w = 1.0;
    // gl_Position.xyz = vertexPosition;

    // gl_Position = mvp * vec4(vertexPosition, 1.0);
    // gl_Position = vec4(vertexPosition, 1.0);

    vec4 vertexWorldPosition = transformMatrix * vec4(vertexPosition, 1.0);
    gl_Position = projMatrix * viewMatrix * vertexWorldPosition;

    uv = vertexUV;

    vertexToLight = lightPosition - vertexWorldPosition.xyz;
    vertexSurface = (transformMatrix * vec4(vertexNormal, 0.0)).xyz;
    vertexToCamera = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - vertexWorldPosition.xyz;
}
