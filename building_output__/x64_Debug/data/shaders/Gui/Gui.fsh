
uniform sampler2D guiTexture;

// From v-shader
varying vec2 uv;

void main()
{
    // vec4 color = texture2D(guiTexture, vec2(uv.x, uv.y));
    vec4 color = texture2D(guiTexture, uv);
    
    gl_FragColor = color;
}
