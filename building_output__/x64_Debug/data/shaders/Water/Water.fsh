uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvTexture;
uniform sampler2D normalTexture;

// const float distortion_factor = 0.1;
const float distortion_factor = 0.08;
uniform float move_speed;

varying vec4 clipSpace;
varying vec2 uv;

// Specular lighting
const vec3 lightColor = vec3(5.0, 5.0, 5.0);
const float reflectivity = 5.0;
const float shineDamper = 2.0;
varying vec3 vertexToCamera;
varying vec3 vertexToLight00;
// varying vec3 vertexToLight01;

void main() {

    // To implement projective texutre mapping for water's
    // reflection and refraction textures
    vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
    vec2 reflectionUV = vec2(ndc.x, -ndc.y);
    vec2 refractionUV = vec2(ndc.x, ndc.y);

    // vec2 dudv_distortion1 = texture2D(dudvTexture, vec2(uv.x + move_speed, uv.y)).rg * 2.0 - 1.0;
    // vec2 dudv_distortion2 = texture2D(dudvTexture, vec2(-uv.x + move_speed, uv.y + move_speed)).rg * 2.0 - 1.0;
    // vec2 dudv_distortion = (dudv_distortion1 + dudv_distortion2) * distortion_factor;
        vec2 dudv_factor = texture2D(dudvTexture, vec2(uv.x + move_speed, uv.y)).rg * 0.1;
        dudv_factor = uv + vec2(dudv_factor.x, dudv_factor.y + move_speed);
        vec2 dudv_distortion = (texture2D(dudvTexture, dudv_factor).rg * 2.0 - 1.0) * distortion_factor;

    // vec4 reflection = texture(reflectionTexture, reflectionUV);
    // vec4 refraction = texture(refractionTexture, refractionUV);
    vec4 reflection = texture(reflectionTexture, reflectionUV + dudv_distortion);
    vec4 refraction = texture(refractionTexture, refractionUV + dudv_distortion);

    vec4 dummy_color = vec4(1.0, 0.4, 0.8, 1.0);

    // normal and specular lighting for water
    vec3 total_specular = vec3(0.0, 0.0, 0.0);
    // vec4 color_from_normal = texture2D(normalTexture, uv + dudv_distortion);  // less like lava
    // vec4 color_from_normal = texture2D(normalTexture, dudv_distortion);    // like lava with dots
        vec4 color_from_normal = texture2D(normalTexture, dudv_factor);  // blocky, wavy
        // vec4 color_from_normal = texture2D(normalTexture, dudv_distortion);  // more random and dense
    {

        vec3 normal = vec3(color_from_normal.r * 2.0 - 1.0, color_from_normal.g * 2.0 - 1.0, color_from_normal.b);
        normal = normalize(normal);

        // vec3 unitVertexSurface = normalize(vertexSurface);
        vec3 unitVertexToLight00 = normalize(vertexToLight00);
        // vec3 vertexReflectedLight00 = reflect(-unitVertexToLight00, unitVertexSurface);
        vec3 vertexReflectedLight00 = reflect(-unitVertexToLight00, normal);

        vec3 unitVertexToCamera = normalize(vertexToCamera);
            float specularFactor00 = dot(vertexReflectedLight00, unitVertexToCamera);
            specularFactor00 = max(specularFactor00, 0.0);
            specularFactor00 = pow(specularFactor00, shineDamper);

            // vec3 specular00 = lightColor * specularFactor00 * reflectivity;
            total_specular = lightColor * specularFactor00 * reflectivity;


        // vec3 unitVertexToLight01 = normalize(vertexToLight01);
        // vec3 vertexReflectedLight01 = reflect(-unitVertexToLight01, normal);
        // vec3 unitVertexToCamera = normalize(vertexToCamera);
        //     float specularFactor01 = dot(vertexReflectedLight01, unitVertexToCamera);
        //     specularFactor01 = max(specularFactor01, 0.0);
        //     specularFactor01 = pow(specularFactor01, shineDamper);

        //     vec3 specular01 = lightColor * specularFactor01 * reflectivity;


        // total_specular = specular00 + specular01;
        // total_specular = specular00;
    }

    // gl_FragColor = dummy_color;
    // gl_FragColor = mix(dummy_color, refraction, 0.5);
    // gl_FragColor = mix(reflection, refraction, 0.4);

    // gl_FragColor = mix(gl_FragColor, vec4(0.0, 0.3, 0.5, 1.0), 0.4);
    // gl_FragColor = texture2D(dudvTexture, uv + dudv_distortion);

    // gl_FragColor = color_from_normal + vec4(specular, 1.0);
    // gl_FragColor = color_from_normal;
    gl_FragColor = dummy_color + vec4(total_specular, 1.0);
}
