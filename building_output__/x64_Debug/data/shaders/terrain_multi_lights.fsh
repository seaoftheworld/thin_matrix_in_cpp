// Inputs
uniform vec3 lightAttenuation[8];
uniform vec3 lightColor[8];

uniform float reflectivity;
uniform float shineDamper;

// Multi-textures and blend-map
uniform sampler2D texture00;  // used as background for terrain
uniform sampler2D texture01;  // 01 ~ 03 used as the other stuff on terrain (path, grass, follower)
uniform sampler2D texture02;  // displayed according to blendmap
uniform sampler2D texture03;
uniform sampler2D blendMap;

// From v-shader
varying vec2 uv;

varying vec3 vertexToLight[8];
varying vec3 vertexToCamera;
varying vec3 vertexSurface;

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

    vec3 unit_VertexSurface = normalize(vertexSurface);
    vec3 unit_VertexToCamera = normalize(vertexToCamera);

    vec3 sum_diffuse = vec3(0.0);
    vec3 sum_specular = vec3(0.0);

    for (int i = 0; i < 8; i++) {
    // for (int i = 0; i < 1; i++) {
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

    vec4 color = vec4(sum_diffuse, 1.0) * blend_color;

    color += vec4(sum_specular, 1.0);
    gl_FragColor = color;
}
