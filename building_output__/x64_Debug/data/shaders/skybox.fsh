uniform samplerCube cube;
varying vec3 UV;

void main() {
    vec4 color = texture(cube, UV);
    gl_FragColor = color;
}
