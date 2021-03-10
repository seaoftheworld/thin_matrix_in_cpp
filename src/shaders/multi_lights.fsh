uniform sampler2D texSampler;

// Inputs
uniform vec3 lightColor[8];
uniform float reflectivity;
uniform float shineDamper;

// uniform float blendAlpha = 1.0f;
// uniform float blendAlpha;

// Passed in here from v-shader
varying vec2 uv;

varying vec3 vertexSurface;
varying vec3 vertexToCamera;
varying vec3 vertexToLight[8];

void main()
{
    // color = texture(texSampler, uv);
    // color.a = blendAlpha;

    // gl_FragColor = texture2D(texSampler, uv);
    // gl_FragColor.a = blendAlpha;

    // gl_FragColor = texture2D(texSampler, uv);
    // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);

    vec3 unitVertexSurface = normalize(vertexSurface);
    vec3 unitVertexToCamera = normalize(vertexToCamera);

    vec3 sum_diffuse = vec3(0.0);
    vec3 sum_specular = vec3(0.0);

    for (int i = 0; i < 8; i++) {
        vec3 unitVertexToLight = normalize(vertexToLight[i]);

        float brightness = dot(unitVertexSurface, unitVertexToLight);
            // brightness = max(raw_brightness, 0.2);
            // vec3 diffuse = brightness * lightColor;
            sum_diffuse += brightness * lightColor[i];


            vec3 vertexReflectedLight = reflect(-unitVertexToLight, unitVertexSurface);

                float specularFactor = dot(vertexReflectedLight, unitVertexToCamera);
                specularFactor = max(specularFactor, 0.0);

                float dampedFactor = pow(specularFactor, shineDamper);
                // vec3 specular = dampedFactor * lightColor;
                sum_specular += dampedFactor * lightColor[i];
    }

    sum_diffuse = max(sum_diffuse, 0.2);

    // vec4 color = vec4(diffuse, 1.0) * texture2D(texSampler, uv);
    // gl_FragColor = color;
    vec4 color = vec4(sum_diffuse, 1.0) * texture2D(texSampler, uv);
    if (color.a < 0.1) {
        discard;
    }

    color = color + vec4(sum_specular, 1.0);
    gl_FragColor = color;
}
