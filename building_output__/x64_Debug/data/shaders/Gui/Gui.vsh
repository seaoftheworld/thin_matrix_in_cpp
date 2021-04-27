attribute vec2 vertexPosition;
// attribute vec2 vertexUV;
// attribute vec3 vertexNormal;

varying vec2 uv;

// uniform mat4 projMatrix;
// uniform mat4 viewMatrix;
uniform mat4 transformMatrix;

uniform vec3 lightPosition;

void main()
{
    // vec4 vertexWorldPosition = transformMatrix * vec4(vertexPosition, 1.0);
    // gl_Position = projMatrix * viewMatrix * vertexWorldPosition;
    gl_Position = transformMatrix * vec4(vertexPosition, 0.0, 1.0);
    
    // uv = vertexUV;
    uv = vec2((1.0 + vertexPosition.x) / 2.0, 1.0 - (1.0 + vertexPosition.y) / 2.0);
}
