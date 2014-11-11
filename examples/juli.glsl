#version 330 core

#texture AAA img/6.jpg

in vec2  uv;
out vec4 color;

uniform float time;
uniform vec2 mouse;
uniform float ration;

uniform sampler1D audioLeft;
uniform sampler1D audioRight;

float left (float val){ return texture(audioLeft , val).r; }
float right(float val){ return texture(audioRight, val).r; }

float _clamp(float val, float minimum, float maximum){ return min(max(val, minimum), maximum); }
float _clamp(float val){ return _clamp(val, 0.0, 1.0); }
vec2  _clamp(vec2  val){ return vec2(_clamp(val.r  ), _clamp(val.g)); }
vec3  _clamp(vec3  val){ return vec3(_clamp(val.rg ), _clamp(val.b)); }
vec4  _clamp(vec4  val){ return vec4(_clamp(val.rgb), _clamp(val.a)); }

uniform int iter = 100;

void main() {
	int i;
	float x, y, d, v, l, r, g, b, y1, y2, intensity, audioVal;
	vec2 p, c, z;

	color = vec4(1);
	p = uv * 2.0 - 1.0;
	audioVal = (left(uv.x) + right(uv.x)) / 6;

//* Fractal inner
	color = vec4(0, audioVal, 0, 1);
//*/

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
//		color.b = _clamp( float(i) * 5.0 / float(iter)			);
//		color.g = _clamp((float(i) * 5.0 / float(iter) - .3) / .7);
//		color.r = _clamp((float(i) * 5.0 / float(iter) - .7) / .3);

		color.rgb = _clamp(vec3(
			(float(i) * 5.0 / float(iter) - 0.7) / 0.3,
			(float(i) * 5.0 / float(iter) - 0.3) / 0.7,
			 float(i) * 5.0 / float(iter)
		));
	}
//*/

/* Wave line
	if(p.y < max(0, audioVal) && p.y > min(0, audioVal)){
		intensity = abs(p.y - audioVal / 2) / abs(audioVal) * 2;
		color.rgb += _clamp(color.rgb * intensity + 1 - intensity - intensity * vec3(-.2, .8, 2.333));

	}
//*/

/* Circle
	c = p - mouse * 2.0 + 1.0;
	d = length(c);
	v = left(abs(atan(c.y, c.x) / 3.141592)) / 3;
	if(d < .2 && d + v >= .2 || d > .2 && d + v <= .2){
		intensity = _clamp(abs(d  + v/2 - .2) / abs(v) * 2);
		color.rgb = _clamp(color.rgb * intensity + 1 - intensity - intensity * vec3(1, 0, .5));
	}
//*/


/* Left
	l = abs(left(uv.y)/2.0);
	if(l >= uv.x){
		intensity = _clamp((uv.x - l / 2) / abs(l) * 2);
		color.rgb += _clamp(color.rgb * intensity + (1 - intensity) * vec3(1,0,0));
	}
//*/


/* Right
	r = abs(right(uv.y)/2.0);
	if(r >= 1.0-uv.x){
		intensity = _clamp((1 - uv.x - r / 2) / abs(r) * 2);
		color.rgb += _clamp(color.rgb * intensity + (1 - intensity) * vec3(1,0,0));
	}
//*/

/* Star
	vec2 center = uv - mouse;
	if(length(center) * 5 + sin(atan(center.x, center.y) * 5.0 + time / 100.0 ) / 3.0 < .5)
		color = vec4(1);
//*/

//* New Waveline
	y1 = y2 = p.y - 0.5;
	r = 0.0;
	b = 0.0;
	float vertColor1 = 0.0, vertColor2 = 0.0;
	const float k = 5.0;
	for( float i = 1.0; i < k; ++i )
	{
		y1 += left (uv.x  * exp(i) / 5) * 0.55;
		y2 += right(uv.x  * exp(i) / 5) * 0.55;
		float fTemp1 = abs(1.0 / (80.0 * k * y1));
		float fTemp2 = abs(1.0 / (80.0 * k * y2));
		vertColor1 += fTemp1 / i;
		vertColor2 += fTemp2 / i;
		r += fTemp1 * i * 0.9;
		b += cos(vertColor2) * sin(fTemp2);
	}

	color.r = _clamp(color.r + r);
	color.b = _clamp(color.b + b);
//*/

/* Aquarell
	for(int i=1;i<50.0;i++){
		vec2 newp = p;
		newp.x += 0.6 / i * sin(float(i) * p.y + time / 3000.0 + 0.3 * i      ) + 1.0;
		newp.y += 0.6 / i * sin(float(i) * p.x + time / 3000.0 + 0.3 *(i+10.0)) - 1.4;
		p = newp;
	}
	color = vec4(
		0.5 * sin(3.0 * p.x) + 0.5,
		0.5 * sin(3.0 * p.y) + 0.5,
		sin(p.x + p.y),
		1.0);
//*/

/* Random
	color = vec4(fract(sin(dot(floor(uv * 64),vec2(12.9898,78.233))) * 43758.5453));
//*/

//* Image
    color.rgb += texture(AAA, uv + vec2(left(uv.y), right(uv.x)) * 0.05).rgb * 0.5;
//*/
}
