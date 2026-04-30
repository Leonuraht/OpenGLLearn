#version 460 core
in vec3 norms;
in vec3 fragpos;
in vec2 texcord;
out vec4 fragcol;

uniform bool islight;

struct Material{
    sampler2D diffuse,specular;
    float shineness;
};
struct Light{
    vec3 ambient,diffuse,specular,pos;
    float constant,linear,quadratic;
};

vec3 lightcol = vec3(1.0f,1.0f,1.0f);

uniform mat4 view;
uniform Light light;
uniform Material material;


void main(){
    if(!islight){
        vec3 texdata = 1.1 * vec3(texture(material.diffuse,texcord));
        vec3 normsn = normalize(norms);
        vec3 lightdir = normalize(light.pos - fragpos);
        float ligdis = length(light.pos - fragpos);
        float attenuation = 1.0f/(1 + 0.09 * ligdis + 0.032 * ligdis * ligdis);
        vec3 viewdir = normalize( -fragpos);
        vec3 reflectdir = reflect(-lightdir,normsn);
        float spec = pow(max(dot(viewdir,reflectdir),0.0f),material.shineness);
        vec3 specular =  (vec3(texture(material.specular,texcord))) * spec * light.specular;
        vec3 diffuse = texdata * max(dot(lightdir,normsn),0.0f) * light.diffuse;
        fragcol = vec4((light.ambient * texdata + diffuse + specular) * attenuation,1.0f);
    }else{
        fragcol = vec4(lightcol,1.0f);
    }
}

