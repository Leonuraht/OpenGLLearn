#version 460 core
layout(location = 0) in vec3 apos;
layout(location = 1) in vec3 norm;

out vec3 norms;
out vec3 fragpos;
out vec3 lightposn;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 invtrans;
uniform bool islight;
uniform vec3 lightpos;

void main(){
    vec4 viewvec = view * transform * vec4(apos,1.0f);
    gl_Position = projection * viewvec;
    fragpos = vec3(viewvec);
    norms = normalize(mat3(invtrans) * norm);
    lightposn = vec3(view * vec4(lightpos,1.0f));
}
