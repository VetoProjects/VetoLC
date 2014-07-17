#version 330 core


in vec2  uv;
in float time;
uniform sampler1D audioLeftData;
uniform sampler1D audioRightData;

out vec4 color;

float left (float val){ return texture(audioLeftData , val).r ; }
float right(float val){ return texture(audioRightData, val).r ; }


#ifdef GL_ES
precision mediump float;
#endif

//noise examples

// Now with mouse controls!

vec2 mouse = vec2(0);

vec3 mod289(vec3 x);
vec4 mod289(vec4 x);
vec4 permute(vec4 x);
vec4 taylorInvSqrt(vec4 r);
float snoise(vec3 v);
float fbm8(vec2 uv);
float fbm4(vec2 uv);
float hash(vec2 uv);

void main( void )
{
	vec2 div = floor(vec2(uv.x*4., uv.y*3.));   //display divisions
//	uv.x *= resolution.x/resolution.y;	    //adjust aspect ratio

	vec2 p = (div.y == 0. ? uv.xx : uv) - mouse;          //use just the x value for graphs on the bottom row		

	
	float noise[4];
	noise[0] = float(hash(floor(p*64.)/64.) > .5);	//blocky boolean noise
	noise[1] = hash(p);			    	//psudorandom hash (extremely high frequency noise)
	noise[2] = fbm4(p);			        //low frequency simplex noise
	noise[3] = fbm8(p);			        //high frequency simplex noise
		
	float result = 0.;
	for(int i = 0; i < 4; i++)
	{
		result = div.x == float(i) ? noise[i] : result;
		result = div.y == 0. ? step(result - fract(uv.y*3.), 0.) : result;
	}
	
	
	color = vec4(result);	
}//sphinx

//psudo-random number generator
float hash(vec2 uv)
{
	uv = vec2( dot(uv,vec2(127.1,311.7)), dot(uv,vec2(269.5,183.3)));
	return abs(fract(sin(uv.x*uv.y)*43758.5453123)-.5)*2.;
}

//fractal brownian motion (noise harmonic)
float fbm4(vec2 uv){
	float n = 0.5;
	float f = 1.0;
	float l = 0.2;
	for (int i=0; i<4; i++)
	{
		n += snoise(vec3(uv*f, 1.0))*l;
		f *= 2.00;
		l *= 0.65;
	}
	return n;	
	
}


//fractal brownian motion (noise harmonic - fewer octaves = smoother)
float fbm8(vec2 uv){
	float n = 0.5;
	float f = 4.0;
	float l = 0.2;
	for (int i=0; i<8; i++)
	{
		n += snoise(vec3(uv*f, 1.0))*l;
		f *= 2.00;
		l *= 0.65;
	}
	return n;	
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description : Array and textureless GLSL 2D/3D/4D simplex 
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
// 

vec3 mod289(vec3 x) 
{
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) 
{
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) 
{
	return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
	return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
{ 
	const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
	const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

	// First corner
	vec3 i  = floor(v + dot(v, C.yyy) );
	vec3 x0 =   v - i + dot(i, C.xxx) ;

	// Other corners
	vec3 g = step(x0.yzx, x0.xyz);
	vec3 l = 1.0 - g;
	vec3 i1 = min( g.xyz, l.zxy );
	vec3 i2 = max( g.xyz, l.zxy );

	//   x0 = x0 - 0.0 + 0.0 * C.xxx;
	//   x1 = x0 - i1  + 1.0 * C.xxx;
	//   x2 = x0 - i2  + 2.0 * C.xxx;
	//   x3 = x0 - 1.0 + 3.0 * C.xxx;
	vec3 x1 = x0 - i1 + C.xxx;
	vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
	vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

	// Permutations
  	i = mod289(i); 
  	vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

	// Gradients: 7x7 points over a square, mapped onto an octahedron.
	// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
	float n_ = 0.142857142857; // 1.0/7.0
	vec3  ns = n_ * D.wyz - D.xzx;

	vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

	vec4 x_ = floor(j * ns.z);
	vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

	vec4 x = x_ *ns.x + ns.yyyy;
	vec4 y = y_ *ns.x + ns.yyyy;
	vec4 h = 1.0 - abs(x) - abs(y);
	
	vec4 b0 = vec4( x.xy, y.xy );
	vec4 b1 = vec4( x.zw, y.zw );

	//vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
	//vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
	vec4 s0 = floor(b0)*2.0 + 1.0;
	vec4 s1 = floor(b1)*2.0 + 1.0;
	vec4 sh = -step(h, vec4(0.0));

	vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
	vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

	vec3 p0 = vec3(a0.xy,h.x);
	vec3 p1 = vec3(a0.zw,h.y);
	vec3 p2 = vec3(a1.xy,h.z);
	vec3 p3 = vec3(a1.zw,h.w);
	
	//Normalise gradients
	vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
	p0 *= norm.x;
	p1 *= norm.y;
	p2 *= norm.z;
	p3 *= norm.w;

	// Mix final noise value
	vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
	m = m * m;
	return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), dot(p2,x2), dot(p3,x3) ) );
  }
