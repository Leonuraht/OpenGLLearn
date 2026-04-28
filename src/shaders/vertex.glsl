
#version 460 core
layout(location = 0) in vec3 apos;
layout(location = 1) in vec3 col;
out vec3 poss;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;


void main(){
    gl_Position = projection * view * transform * vec4(apos,1.0f);
    poss = col;
}
