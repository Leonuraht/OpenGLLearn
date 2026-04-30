#version 460 core
in vec3 normvec;
out vec4 fragcol;

uniform bool islight;
uniform vec3 color;
uniform vec3 lightpos;
uniform vec3 fragpos;

vec3 lightcolor = vec3(0.0f,1.0f,1.0f);

float ambient  = 0.1f;

void main(){
    if(!islight){
        vec3 lightdir = normalize(lightpos - fragpos);
        vec3 diffuse = max(dot(lightdir,normvec),0.0f) * lightcolor;
        fragcol = vec4((ambient + diffuse) * color,1.0f);
    }else{
        fragcol = vec4(lightcolor,1.0f);
    }
}

