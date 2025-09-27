#version 450 core

in vec4 color;
in vec3 normal;
in vec3 world_pos;
in vec2 tex_coord;

out vec4 FragColor;

uniform vec3 light_pos;
uniform vec3 camera_pos;
uniform sampler2D ourTexture;
uniform int is_textured;


void main()
{
    vec3 norm = normalize(normal);
    float ambient_c, diffuse_c, specular_c;
    vec3 ambient, diffuse, specular;

    vec3 frag_color;
    if(is_textured != 0)
        frag_color = texture(ourTexture, tex_coord).xyz;
    else
        frag_color = color.xyz;

    ambient_c = 0.35;
    ambient = ambient_c * frag_color;

    float light_intensity = 60.0;
    vec3 light_dir = normalize(light_pos - world_pos);
    float light_dist = abs(length(light_pos - world_pos));
    float attenuation = 1 / (pow(light_dist, 1.5));
    diffuse_c = max(dot(norm, light_dir), 0.0) * light_intensity;
    diffuse_c *= attenuation;
    diffuse = diffuse_c * frag_color;


    float spec_strength = 0.8;
    vec3 view_dir = normalize(camera_pos - world_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    specular_c = pow(max(dot(view_dir, reflect_dir), 0.0), 8) * spec_strength;
    specular_c *= attenuation;
    specular = specular_c * vec3(1.0, 1.0, 1.0);

    FragColor = vec4((ambient + diffuse + specular), 1.0);
}