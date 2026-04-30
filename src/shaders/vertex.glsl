#version 460 core
layout(location = 0) in vec3 apos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 texcor;

out vec3 norms;
out vec3 fragpos;
out vec2 texcord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 invtrans;
uniform bool islight;

void main(){
    vec4 viewvec = view * transform * vec4(apos,1.0f);
    gl_Position = projection * viewvec;
    fragpos = vec3(viewvec);
    texcord = texcor;
    if(!islight) norms = mat3(invtrans) * norm;
}
