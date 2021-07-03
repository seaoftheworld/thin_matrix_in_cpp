attribute vec3 vertexPosition;
attribute vec2 vertexUV;
attribute vec3 vertexNormal;

varying vec2 uv;

varying vec3 vertexSurface;
varying vec3 vertexToLight;
varying vec3 vertexToCamera;

// uniform float tFloat;

// uniform mat4 mvp;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformMatrix;

uniform vec3 lightPosition;

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

    // uv = vertexUV * 4.0;
    uv = vertexUV;

    vertexSurface = (transformMatrix * vec4(vertexNormal, 0.0)).xyz;
    vertexToLight = lightPosition - vertexWorldPosition.xyz;
    vertexToCamera = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - vertexWorldPosition.xyz;
}
