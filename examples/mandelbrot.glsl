#version 330 core

in vec2  uv;
out vec4 color;

uniform float time;
uniform vec2 mouse;
uniform float ration;

uniform sampler1D audioLeft, audioRight;

float left (float val){ return texture(audioLeft , val).r; }
float right(float val){ return texture(audioRight, val).r; }

float scale = 4;
int iter = 10;

void main() {
	vec2 center = mouse - 0.5;
	color= vec4(0,0,0,1);
	vec2 z, c;

	c.x = 1.3333 * (uv.x - 0.5 - center.x) * scale;
	c.y = (uv.y - 0.5 - center.y) * scale;

	int i;
	z = c;
	for(i=0; i < iter; ++i) {
		float x = (z.x * z.x - z.y * z.y) + c.x;
		float y = (z.y * z.x + z.x * z.y) + c.y;

		if((x * x + y * y) > 4.0)
			break;
		z.x = x;
		z.y = y;
	}
	if(i < iter)
	color.r = float(i)/10.0;
}
