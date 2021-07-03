attribute vec3 vertexPosition;
attribute vec2 vertexUV;
attribute vec3 vertexNormal;

varying vec2 uv;
varying vec3 normal;

// uniform float tFloat;
// uniform mat4 mvp;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformMatrix;

void main()
{
    // gl_Position.xyz = vertexPosition;
    // gl_Position.x += tFloat;
    // gl_Position.w = 1.0;
    // gl_Position.xyz = vertexPosition;

    // gl_Position = mvp * vec4(vertexPosition, 1.0);
    // gl_Position = vec4(vertexPosition, 1.0);

    // gl_Position = transformMatrix * vec4(vertexPosition, 1.0);
    vec4 worldPosition = transformMatrix * vec4(vertexPosition, 1.0);
    gl_Position = projMatrix * viewMatrix * worldPosition;

    uv = vertexUV;
    normal = vertexNormal;
}
