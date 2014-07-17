#version 330 core


in vec2  uv;
in float time;
uniform sampler1D audioLeftData;
uniform sampler1D audioRightData;

uniform int iter = 100;
out vec4 color;

float left (float val){ return texture(audioLeftData , val).r ; }
float right(float val){ return texture(audioRightData, val).r ; }

float clamp(float val){ return clamp(val, 0.0, 1.0); }
vec2  clamp(vec2  val){ return clamp(val, 0.0, 1.0); }
vec3  clamp(vec3  val){ return clamp(val, 0.0, 1.0); }
vec4  clamp(vec4  val){ return clamp(val, 0.0, 1.0); }

void main() {
	int i;
	float x, y, d, v, l, r, intensity, audioVal;
	vec2 p, c, z;	

	color = vec4(1);
	p = uv * 2.0 - 1.0;
	audioVal = (left(uv.x) + right(uv.x)) / 6;

// Fractal inner
	color = vec4(0, audioVal, 0, 1);

// Fractal
//*
	z = p * vec2(1.2,1.2);
	c = vec2(cos(time/1200.0),sin(time/1000.0));
	for(i=0; i < iter; ++i) {
		x = (z.x * z.x - z.y * z.y) + c.x + left(c.x) / 5;
		y = (z.x * z.y + z.x * z.y) + c.y + left(c.y) / 5;

		if((x * x + y * y) > 4.0)
			break;
		z.x = x;
		z.y = y;
	}
	if(i < iter){
		color.b = clamp( float(i) * 5.0 / float(iter)			);
		color.g = clamp((float(i) * 5.0 / float(iter) - .3) / .7);
		color.r = clamp((float(i) * 5.0 / float(iter) - .7) / .3);
	}
//*/

//* Wave line
//	audioVal = abs(audioVal);
	if(p.y < max(0, audioVal) && p.y > min(0, audioVal)){
		intensity = abs(p.y - audioVal / 2) / abs(audioVal) * 2;
		color.rgb *= intensity;
		color.rgb += clamp(1 - intensity - intensity * vec3(-.2, .8, 2.333));
		
	}

// Circle
/*
	c = vec2(sin(time/1000), cos(time/1000)) * .7 - p;
	d = length(c);
	v = left(abs(atan(c.y, c.x) / 3.141592)) / 3;
	if(d < .2 && d + v >= .2 || d > .2 && d + v <= .2){
		intensity = abs(d  + v/2 - .2) / abs(v) * 2;
		color.rgb *= intensity;
		color.rgb += clamp(1 - intensity - intensity * vec3(1, 0, .5));
	}
//*/


/* Left
	l = abs(left(uv.y)/2.0);
	if(l >= uv.x){
		float intensity = (uv.x - l / 2) / abs(l) * 2;
		color.rgb *= intensity;
		color.rgb += clamp((1 - intensity) * vec3(1,0,0));
	}
//*


/* Right
	r = abs(right(uv.y)/2.0);
	if(abs(r/2.0) >= 1-uv.x){
		intensity = clamp((1 - uv.x - r / 2) / abs(r) * 2, 0, 1);
		color.rgb *= intensity;
		color.rgb += (1 - intensity) * clamp(vec3(1,0,0));
	}
//*/

// Star
/*
	if(length(p) + sin(atan(p.y, p.x)*5 + time/100)/3 < .5)
		color = vec4(1);
//*/
}
