#version 330 core


in vec2  uv;
in float time;
// uniform float[1764] audioData;
uniform sampler1D audioData;

//uniform highp vec2 c = vec2(.1,.1);
uniform int iter = 100;
out vec4 color;

float aud(float val){ return texture(audioData, val).r ; }

void main() {
	color = vec4(1);
	vec2 p = 1.0 - uv * 2.0;
	float audioVal = aud(uv.x) / 3;

// Star
//	if(length(p) + sin(atan(p.y, p.x)*5)/3 < .5)
//		return;


// Fractal inner
	color = vec4(0, audioVal, 0, 1);

// Fractal
	vec2 z = p * vec2(1.2,1.2);
	vec2 c = vec2(cos(time/1200.0),sin(time/1000.0));
//	vec2 c = vec2(sin(time / 1000.0), cos(time / 500.0));
	int i;
	for(i=0; i < iter; ++i) {
		float x = (z.x * z.x - z.y * z.y) + c.x + aud(c.x) / 5;
		float y = (z.x * z.y + z.x * z.y) + c.y + aud(c.y) / 5;

		if((x * x + y * y) > 4.0)
			break;
		z.x = x;
		z.y = y;
	}
	if(i < iter){
		color.b = clamp(float(i) * 5.0 / float(iter), 0, 1);
		color.g =clamp((float(i) * 5.0 / float(iter) - .3) / .7, 0, 1);
		color.r = clamp((float(i) * 5.0 / float(iter) - .7) / .3, 0, 1);
	}


// Wave line
	if(p.y < max(0, audioVal) && p.y > min(0, audioVal)){
		float intensity = abs(p.y - audioVal/2) / abs(audioVal) * 2;
		color.rgb *= intensity;
		color.rgb += 1-intensity - intensity* vec3(2.333, 0.428571429, 0);
		
	}

// Circle
/*
	c = p + vec2(sin(time/1000), cos(time/1000));
	float d = length(c);
	float v = aud(abs(atan(c.y, c.x) / 3.141592)) / 10 + .4;
	if(d < v){
		float intensity = length(c) / v;
		color.rgb *= intensity;
		color.rgb += 1 - intensity - intensity * vec3(1, 0, .5);
		return;
	}*/
}