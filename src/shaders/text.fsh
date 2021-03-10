varying vec2 uv;

uniform sampler2D texSampler;
uniform float blendAlpha;

void main()
{
    // gl_FragColor = vec4( vec3(texture2D(texSampler, uv).r), texture2D(texSampler, uv).g );
    gl_FragColor = texture2D(texSampler, uv);

    // gl_FragColor.a = blendAlpha;
    if (gl_FragColor.a >= 0.4) {
        // non-alpha pixel
        // gl_FragColor.xyz = vec3(0.6, 0.6, 0.8);
        
        // gl_FragColor.a = 1.0;
        gl_FragColor.a = blendAlpha;
    }
    else {
        // alpha pixel
    }
}
