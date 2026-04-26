#version 460 core
in vec3 poss;
in vec2 texcord;
out vec4 fragcol;
uniform float color;
uniform sampler2D texdata;
uniform sampler2D texll;

void main(){
    fragcol = mix(texture(texdata,texcord),texture(texll,texcord),2*color) * color + vec4(poss.x + color,poss.yz,0.0f);
}

