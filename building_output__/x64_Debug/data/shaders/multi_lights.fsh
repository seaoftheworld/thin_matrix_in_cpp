uniform sampler2D texSampler;

// Inputs
uniform vec3 lightColor[8];
// uniform vec3 lightColor0;
// uniform vec3 lightColor1;

uniform float reflectivity;
uniform float shineDamper;

// uniform float blendAlpha = 1.0f;
// uniform float blendAlpha;

// Passed in here from v-shader
varying vec2 uv;

varying vec3 vertexSurface;
varying vec3 vertexToCamera;

varying vec3 vertexToLight[8];
// varying vec3 vertexToLight0;
// varying vec3 vertexToLight1;

void main()
{
    // color = texture(texSampler, uv);
    // color.a = blendAlpha;

    // gl_FragColor = texture2D(texSampler, uv);
    // gl_FragColor.a = blendAlpha;

    // gl_FragColor = texture2D(texSampler, uv);
    // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);

    vec3 unit_VertexSurface = normalize(vertexSurface);
    vec3 unit_VertexToCamera = normalize(vertexToCamera);

    vec3 sum_diffuse = vec3(0.0);
    vec3 sum_specular = vec3(0.0);

    for (int i = 0; i < 8; i++) {
        vec3 unit_VertexToLight = normalize(vertexToLight[i]);

        float brightness = dot(unit_VertexSurface, unit_VertexToLight);
            brightness = max(brightness, 0.0);
            sum_diffuse = sum_diffuse + brightness * lightColor[i];


            vec3 vertexReflectedLight = reflect(-unit_VertexToLight, unit_VertexSurface);
                float specularFactor = dot(vertexReflectedLight, unit_VertexToCamera);
                specularFactor = max(specularFactor, 0.0);
                float dampedFactor = pow(specularFactor, shineDamper);
                sum_specular = sum_specular + dampedFactor * lightColor[i];
    }

    // {
    //     vec3 unit_VertexToLight = normalize(vertexToLight0);
    //     float brightness = dot(unit_VertexSurface, unit_VertexToLight);
    //         brightness = max(brightness, 0.0);
    //         // vec3 diffuse = brightness * lightColor;
    //         sum_diffuse = sum_diffuse + brightness * lightColor0;


    //         // vec3 vertexReflectedLight = reflect(-unit_VertexToLight, unit_VertexSurface);
    //         //     float specularFactor = dot(vertexReflectedLight, unit_VertexToCamera);
    //         //     specularFactor = max(specularFactor, 0.0);
    //         //     float dampedFactor = pow(specularFactor, shineDamper);
    //         //     // vec3 specular = dampedFactor * lightColor;
    //         //     sum_specular = sum_specular + dampedFactor * lightColor[i];

    //     unit_VertexToLight = normalize(vertexToLight1);
    //     brightness = dot(unit_VertexSurface, unit_VertexToLight);
    //         brightness = max(brightness, 0.0);
    //         // vec3 diffuse = brightness * lightColor;
    //         sum_diffuse = sum_diffuse + brightness * lightColor1;
    // }

    {
        // vec3 unit_VertexToLight = normalize(vertexToLight[1]);
        // float brightness = dot(unit_VertexSurface, unit_VertexToLight);
        //     brightness = max(brightness, 0.0);
        //     // vec3 diffuse = brightness * lightColor;
        //     sum_diffuse = sum_diffuse + brightness * lightColor[1];
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
