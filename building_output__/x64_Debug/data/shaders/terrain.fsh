uniform sampler2D texture00;
uniform sampler2D texture01;
uniform sampler2D texture02;
uniform sampler2D texture03;
uniform sampler2D blendMap;

// From v-shader
varying vec2 uv;

varying vec3 vertexSurface;
varying vec3 vertexToLight;
varying vec3 vertexToCamera;

// Inputs
uniform vec3 lightColor;

uniform float reflectivity;
uniform float shineDamper;

// uniform float blendAlpha = 1.0f;
// uniform float blendAlpha;

void main()
{
    vec4 blendMapColor = texture2D(blendMap, uv);
    float texture00_proportion = 1.0 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);

    vec2 tiled_uv = uv * 4.0;
    vec4 texture00_color = texture2D(texture00, tiled_uv) * texture00_proportion;
    vec4 texture01_color = texture2D(texture01, tiled_uv) * blendMapColor.r;
    vec4 texture02_color = texture2D(texture02, tiled_uv) * blendMapColor.g;
    vec4 texture03_color = texture2D(texture03, tiled_uv) * blendMapColor.b;

    vec4 blend_color = texture00_color + texture01_color + texture02_color + texture03_color;
    // color = texture(texSampler, uv);
    // color.a = blendAlpha;

    // gl_FragColor = texture2D(texSampler, uv);
    // gl_FragColor.a = blendAlpha;

    // gl_FragColor = texture2D(texSampler, uv);
    // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);

        vec3 unitVertexSurface = normalize(vertexSurface);
        vec3 unitVertexToLight = normalize(vertexToLight);

        float brightness = dot(unitVertexSurface, unitVertexToLight);
            brightness = max(brightness, 0.2);
            vec3 diffuse = brightness * lightColor;
        vec4 color = vec4(diffuse, 1.0) * blend_color;
        // vec4 color = blend_color;

    // if (color.a < 0.1) {
    //     discard;
    // }
    // gl_FragColor = color;

        vec3 vertexReflectedLight = reflect(-unitVertexToLight, unitVertexSurface);
        vec3 unitVertexToCamera = normalize(vertexToCamera);
            float specularFactor = dot(vertexReflectedLight, unitVertexToCamera);
            specularFactor = max(specularFactor, 0.0);

            float dampedFactor = pow(specularFactor, shineDamper);
            vec3 specular = dampedFactor * lightColor;

        // gl_FragColor = vec4(diffuse, 1.0) * texture2D(texSampler, uv) + vec4(specular, 1.0);

        // vec4 color = vec4(diffuse, 1.0) * texture2D(texSampler, uv);
        // if (color.a < 0.1) {
        //     discard;
        // }
        color = color + vec4(specular, 1.0);
        gl_FragColor = color;

        // gl_FragColor = color;
}
