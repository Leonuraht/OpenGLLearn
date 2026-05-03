#version 460 core
in vec3 norms;
in vec3 fragpos;
in vec2 texcord;
out vec4 fragcol;

struct Material {
    sampler2D diffuse, specular;
    float shineness;
};
struct Light {
    vec3 ambient, diffuse, specular, pos, dir;
    float constant, linear, quadratic;
};

vec3 lightcol = vec3(1.0f, 1.0f, 1.0f);
float lightinten = 1.0f;
float lightcutoff = cos(radians(34));

uniform bool islight;
uniform mat4 view;
uniform Light light[4];
uniform Light Dirlight;
uniform Material material;

vec3 calcDirLight(Light light, vec3 norm, vec3 viewdir) {
    vec3 ldr = normalize(-light.dir);
    float diff = max(dot(ldr, norm), 0.0f);
    vec3 reflecti = reflect(-ldr, norm);
    float spec = pow(max(dot(reflecti, viewdir), 0.0f), material.shineness);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texcord));
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, texcord));
    vec3 specular = spec * light.specular * vec3(texture(material.specular, texcord));
    return (ambient + diffuse + specular);
}

vec3 calcPoiLight(Light light, vec3 norm, vec3 viewdir, vec3 fragpos) {
    vec3 ldr = normalize(light.pos - fragpos);
    float diff = max(dot(ldr, norm), 0.0f);
    vec3 reflecti = reflect(-ldr, norm);
    float spec = pow(max(dot(reflecti, viewdir), 0.0f), material.shineness);
    float len = length(light.pos - fragpos);
    float attenuation = 1 / (1.0 + len * 0.09 + len * len * 0.032);
    diff *= attenuation;
    spec *= attenuation;

    vec3 ambient = attenuation * light.ambient * vec3(texture(material.diffuse, texcord));
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, texcord));
    vec3 specular = spec * light.specular * vec3(texture(material.specular, texcord));
    return (ambient + specular + diffuse);
}

vec3 calcSpotLight(Light light, vec3 norm, vec3 viewdir, float theta) {
    vec3 ligdir = normalize(light.pos - fragpos);
    float diff = max(dot(ligdir, norm), 0.0);
    vec3 reflecti = reflect(-ligdir, norm);
    float spec = pow(max(dot(viewdir, reflecti), 0.0), material.shineness);
    float len = length(ligdir);
    float attenuation = 1.0 / (1.0 + 0.09 * len + 0.032 * len * len);
    diff *= attenuation;
    spec *= attenuation;
    float intensity = clamp((cos(radians(theta)) - 0.82) / 0.09, 0.0, 1.0);
    intensity = intensity * intensity * (3.0 - 2.0 * intensity);

    vec3 ambient = attenuation * light.ambient * vec3(texture(material.diffuse, texcord));
    vec3 diffuse = intensity * diff * light.diffuse * vec3(texture(material.diffuse, texcord));
    vec3 specular = intensity * spec * light.specular * vec3(texture(material.specular, texcord));
    return (ambient + diffuse + specular);
}

void main() {
    if (!islight) {
        vec3 normsn = normalize(norms);
        vec3 viewdir = normalize(-fragpos);
        vec3 result = calcDirLight(Dirlight, normsn, viewdir);
        for (int i = 0; i < 4; i++) {
            result += calcPoiLight(light[i], normsn, viewdir, fragpos);
        }
        fragcol = vec4(pow(result, vec3(1.0 / 2.2)), 1.0f);
    } else {
        fragcol = vec4(lightcol, 1.0f);
    }
}
