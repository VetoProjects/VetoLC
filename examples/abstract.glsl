#version 330 core


in vec2  uv;
in float time;
uniform sampler1D audioLeftData;
uniform sampler1D audioRightData;

vec2 mouse = vec2(0);

out vec4 color;

float left (float val){ return texture(audioLeftData , val).r ; }
float right(float val){ return texture(audioRightData, val).r ; }

// hacked from a Fragmentaium example
//
// http://glsl.heroku.com/e#14401.0
// http://glsl.heroku.com/e#9904.1
//
// power-2 mandelbrot with rotation not about origin (mouse position) + constant rotation
// p' = r(p-c)^2 + c + p0

#ifdef GL_ES
precision highp float;
#endif

const float speed = 0.005;

const float Radius = 1.0033200;
const float Power  = 0.60000;
const float Divider = 35.00000;

const float R = 0.4;
const float G = 0.4;
const float B = 0.9;

const int Iterations = 32;

vec2 rot = vec2(1.0, 0.0);

vec3 doColor(int i, vec2 p, float d)
{
  // The color scheme here is based on one
  // from Inigo Quilez's Shader Toy:
  float co = float(i) + 1.0 - log2(.5*log2(dot(p,p)));
  co = sqrt(co/256.0);
  float  co2 = d * Divider;
  float fac = clamp(1.0/pow(co2,Power),0.0,1.0);

  return fac*vec3( .3+.5*cos(6.2831*co+R),
		   .3+.5*cos(6.2831*co+G),
		   .3+(abs(sin(time * speed)) * .2) + .5 *cos(6.2831*co+B));
}

// double angle wrt positive-x axis
vec2 px2(vec2 z)
{
  float x = (z.x + z.y)*(z.x - z.y);
  float y = z.x * z.y;
  return vec2(x, y+y);
}

vec2 cmul(vec2 a, vec2 b) 
{
  return vec2(a.x*b.x- a.y*b.y, a.x*b.y+a.y*b.x);
}

vec2 julia = vec2(.0, .0);
vec3 colorq(vec2 c, vec2 m, vec2 orbit, float radius)
{
  vec2 p = julia;
  vec2 pOld = vec2(0.0);
  vec2 z;
  int  i = 0;
  float dist = 10000.0;
	
  for (int i = 0; i < Iterations; i++) {
    pOld = p;
    p = cmul(px2(p),rot)-c +m;
    dist = min(dist, abs(length(p - orbit)) - radius );
    //dist = min(dist, abs(length(p - orbit) - Radius));
	  
    if (dot(p,p)> 50.0) {
       return doColor(i, p, dist);
    }
  }
  return vec3(0.0);
}

vec3 colorql(vec2 c, vec2 m, vec2 orbit, float radius)
{
  vec2 p = julia;
  vec2 pOld = vec2(0.0);
  vec2 z;
  int  i = 0;
  float dist = 10000.0;
	
  for (int i = 0; i < Iterations; i++) {
    pOld = p;
    p = cmul(px2(p),rot)-c +m;
    dist = min(dist, abs(p.x - orbit.x) - radius);
    //dist = min(dist, abs(length(p - orbit) - Radius));
	  
    if (dot(p,p)> 50.0) {
       return doColor(i, p, dist);
    }
  }
  return vec3(0.0);
}



void main(void) 
{
  vec2 p = uv * 2.0 - vec2(0.5, 0.0);
  vec2 mouse2 = vec2(.56, .5);
  vec2 mousePosition = mouse;
  mousePosition = mousePosition * 2.0 - vec2(0.5, 0.0);
  float t = time * speed * 0.005;

  julia.x = 1.0 * sin(time * speed * 0.3);
  julia.y = 0.5 * cos(time * speed * .1);
	
  rot = vec2(cos(t),sin(t));

	vec2 orbit = vec2(2.1, 0.5);
	orbit.y = sin(time * speed * .2) * 4.;
	orbit.x = cos(time * speed * .28) * 6.;
  vec3 c = colorq(p, mousePosition, orbit, abs(sin(time * speed * .12)) * Radius);
	
	orbit.y = sin(time * speed * .29) * -2.;
	orbit.x = cos(time * speed * .48) * 2. + 1.;
  vec3 c2 = colorql(p, mousePosition, orbit, 0.1);
  
  //color.rgb = vec3(max(c.r, c2.r), min(c.g, c2.g), max(c.b, c2.b));
  color.rgb = (c * c2) + (c + c2) * .75;
  color.a   = 1.0;
}
