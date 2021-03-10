attribute vec3 vertexPosition;
attribute vec2 vertexUV;

uniform mat4 mvp;
varying vec2 uv;

void main()
{
    gl_Position = mvp * vec4(vertexPosition, 1.0);
    uv = vertexUV;
}
