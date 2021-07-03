// out vec4 color;
varying vec2 uv;

varying vec3 vertexSurface;
varying vec3 vertexToLight;
varying vec3 vertexToCamera;

uniform sampler2D texSampler;

uniform vec3 lightColor;

uniform float reflectivity;
uniform float shineDamper;

// uniform float blendAlpha = 1.0f;
// uniform float blendAlpha;

void main()
{
    // color = texture(texSampler, uv);
    // color.a = blendAlpha;

    // gl_FragColor = texture2D(texSampler, uv);
    // gl_FragColor.a = blendAlpha;

    // gl_FragColor = texture2D(texSampler, uv);
    // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);

    vec3 unitVertexSurface = normalize(vertexSurface);
    vec3 unitVertexToLight = normalize(vertexToLight);

    float dot_result = dot(unitVertexSurface, unitVertexToLight);
        float brightness = max(dot_result, 0.2);
        vec3 diffuse = brightness * lightColor;

    // gl_FragColor = vec4(diffuse, 1.0) * texture2D(texSampler, uv);
    vec4 color = vec4(diffuse, 1.0) * texture2D(texSampler, uv);
    if (color.a < 0.1) {
        discard;
    }
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
}
