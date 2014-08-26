#version 330 core

in vec2  uv;

uniform float time;
uniform vec2 mouse;
uniform float ration;

uniform sampler1D audioLeftData;
uniform sampler1D audioRightData;

out vec4 color;

float left (float val){ return texture(audioLeftData , val).r ; }
float right(float val){ return texture(audioRightData, val).r ; }

void main() {
    // Your code comes here
}
