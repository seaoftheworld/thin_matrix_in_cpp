varying vec2 uv;
// out vec4 color;

uniform sampler2D texSampler;
// uniform float blendAlpha = 1.0f;
uniform float blendAlpha;

void main()
{
    // color = texture(texSampler, uv);
    // color.a = blendAlpha;

    // gl_FragColor = texture2D(texSampler, uv);
    // gl_FragColor.a = blendAlpha;

    // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    // gl_FragColor = texture2D(texSampler, uv);

    vec4 color = texture2D(texSampler, uv);
    if (color.a < 0.1) {
        discard;
    }
    
    gl_FragColor = color;
}
