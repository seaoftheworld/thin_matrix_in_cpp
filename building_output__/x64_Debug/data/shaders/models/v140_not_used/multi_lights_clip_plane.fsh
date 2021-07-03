#version 140

uniform sampler2D texSampler;

// Inputs
uniform vec3 lightAttenuation[8];
uniform vec3 lightColor[8];

uniform float reflectivity;
uniform float shineDamper;

// From v-shader
in vec2 uv;
in vec3 vertexToCamera;
in vec3 vertexSurface;
in vec3 vertexToLight[8];

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

    // Cause c++ sw to crash !!! TODO: why c++ sw crash tobe debugged !!!
    //
    // // vec4 color = vec4(diffuse, 1.0) * texture2D(texSampler, uv);
    // // gl_FragColor = color;
    // vec4 color = vec4(sum_diffuse, 1.0) * texture2D(texSampler, uv);
    // if (color.a < 0.1) {
    //     discard;
    // }

    vec3 sum_diffuse = vec3(0.0);
    vec3 sum_specular = vec3(0.0);

    for (int i = 0; i < 8; i++) {
        vec3 unit_VertexToLight = normalize(vertexToLight[i]);
        float lightDistance = length(vertexToLight[i]);
        float attFactor = lightAttenuation[i].x + lightDistance * lightAttenuation[i].y + lightDistance * lightDistance * lightAttenuation[i].z;

        float brightness = dot(unit_VertexSurface, unit_VertexToLight);
            brightness = max(brightness, 0.2);
            sum_diffuse = sum_diffuse + brightness * lightColor[i] / attFactor;


            vec3 vertexReflectedLight = reflect(-unit_VertexToLight, unit_VertexSurface);
                
                float specularFactor = dot(vertexReflectedLight, unit_VertexToCamera);
                specularFactor = max(specularFactor, 0.0);

                float dampedFactor = pow(specularFactor, shineDamper);
                sum_specular = sum_specular + dampedFactor * lightColor[i] / attFactor;
    }

    // vec4 color = vec4(sum_diffuse, 1.0) * texture2D(texSampler, uv);
    vec4 color = vec4(sum_diffuse, 1.0) * texture(texSampler, uv);
    if (color.a < 0.1) {
        discard;
    }

    color = color + vec4(sum_specular, 1.0);
    gl_FragColor = color;
}
