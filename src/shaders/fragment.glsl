#version 460 core
in vec3 norms;
in vec3 fragpos;
in vec3 lightposn;

out vec4 fragcol;

uniform bool islight;
uniform vec3 color;

vec3 lightcolor = vec3(1.0f,1.0f,1.0f);
float ambient  = 0.1f;

void main(){
    if(!islight){
        vec3 lightdir = normalize(lightposn - fragpos);
        vec3 viewdir = normalize( -fragpos);
        vec3 reflectdir = reflect(-lightdir,norms);
        float spec = pow(max(dot(viewdir,reflectdir),0.0f),32);
        vec3 specular = 0.7 * spec * lightcolor;
        vec3 diffuse = max(dot(lightdir,norms),0.0f) * lightcolor;
        fragcol = vec4((ambient + diffuse + specular) * color,1.0f);
    }else{
        fragcol = vec4(lightcolor,1.0f);
    }
}

