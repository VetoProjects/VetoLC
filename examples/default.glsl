#version 330 core


in vec2  uv;
uniform float time;
uniform sampler1D audioLeft;
uniform sampler1D audioRight;

uniform int iter = 100;
out vec4 color;

float left (float val){ return texture(audioLeft , val).r; }
float right(float val){ return texture(audioRight, val).r; }

float _clamp(float val, float minimum, float maximum){ return min(max(val, minimum), maximum); }
float _clamp(float val){ return _clamp(val, 0.0, 1.0); }
vec2  _clamp(vec2  val){ return vec2(_clamp(val.r  ), _clamp(val.g)); }
vec3  _clamp(vec3  val){ return vec3(_clamp(val.rg ), _clamp(val.b)); }
vec4  _clamp(vec4  val){ return vec4(_clamp(val.rgb), _clamp(val.a)); }

void main() {
	int i;
	float x, y, d, v, l, r, intensity, audioVal;
	vec2 p, c, z;

	color = vec4(0.0, 0.0, 0.0, 1.0);
	p = uv * 2.0 - 1.0;
	audioVal = (left(uv.x) + right(uv.x)) / 6;

// Fractal inner
	color.g = audioVal;

//* Fractal
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
		color.b = _clamp( float(i) * 5.0 / float(iter)			);
		color.g = _clamp((float(i) * 5.0 / float(iter) - .3) / .7);
		color.r = _clamp((float(i) * 5.0 / float(iter) - .7) / .3);
	}
//*/

/* Wave line
	audioVal = abs(audioVal);
	if(p.y < max(0, audioVal) && p.y > min(0, audioVal)){
		intensity = abs(p.y - audioVal / 2) / abs(audioVal) * 2;
		color.rgb *= intensity;
		color.rgb += _clamp(1 - intensity - intensity * vec3(-.2, .8, 2.333));

	}
//*/


/* Circle
	c = p;//vec2(sin(time/1000), cos(time/1000)) * .7 - p;
	d = length(c);
	v = left(abs(atan(c.y, c.x) / 3.141592)) / 3;
	if(d < .2 && d + v >= .2 || d > .2 && d + v <= .2){
		intensity = abs(d  + v/2 - .2) / abs(v) * 2;
		color.rgb *= intensity;
		color.rgb += _clamp(1 - intensity - intensity * vec3(1, 0, .5));
	}
//*/


//* Left
	l = abs(left(uv.y) / 2.0);
	if(l >= uv.x){
		intensity = _clamp((uv.x - l / 2) / l * 2);
		color.rgb *= intensity;
		color.rgb += (1 - intensity) * vec3(1,0,0);
	}
//*


//* Right
	r = abs(right(uv.y)/2.0);
	if(r >= 1 - uv.x){
		intensity = _clamp((1 - uv.x - r / 2) / r * 2);
		color.rgb *= intensity;
		color.rgb += (1 - intensity) * vec3(1,0,0);
	}
//*/

/* Star

	if(length(p) + sin(atan(p.y, p.x)*5 + time/100)/3 < .5)
		color = vec4(1);
//*/
}
