#version 450 core

in vec4 color;
in vec3 normal;
in vec3 world_pos;

out vec4 FragColor;

uniform vec3 light_pos;
uniform vec3 camera_pos;


void main()
{
    vec3 norm = normalize(normal);
    float ambient_c, diffuse_c, specular_c;
    vec3 ambient, diffuse, specular;

    ambient_c = 0.15;
    ambient = ambient_c * color.xyz;

    float light_intensity = 60.0;
    vec3 light_dir = normalize(light_pos - world_pos);
    float light_dist = abs(length(light_pos - world_pos));
    float attenuation = 1 / (pow(light_dist, 1.5));
    diffuse_c = max(dot(norm, light_dir), 0.0) * light_intensity;
    diffuse_c *= attenuation;
    diffuse = diffuse_c * color.xyz;


    float spec_strength = 0.8;
    vec3 view_dir = normalize(camera_pos - world_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    specular_c = pow(max(dot(view_dir, reflect_dir), 0.0), 8) * spec_strength;
    specular_c *= attenuation;
    specular = specular_c * vec3(1.0, 1.0, 1.0);

    FragColor = vec4((ambient + diffuse + specular), 1.0);
}