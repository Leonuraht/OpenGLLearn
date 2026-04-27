#version 460 core
layout(location = 0) in vec3 apos;
layout(location = 1) in vec3 col;
layout(location = 2) in vec2 texs;
out vec3 poss;
out vec2 texcord;
uniform float color;
uniform float mov;
uniform mat4 transform;
uniform mat4 proj;
uniform mat4 view;

void main(){
    gl_Position = proj * view * transform * vec4(apos.xyz,1.0f);
    poss = col;
    texcord = texs;
}
