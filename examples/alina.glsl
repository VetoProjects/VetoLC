#version 330 core

#texture AAA img/6.jpg

in vec2  uv;
out vec4 color;

uniform float time;
uniform vec2 mouse;
uniform float ration;

uniform sampler1D audioLeft;
uniform sampler1D audioRight;

float _clamp(float val, float minimum, float maximum){ return min(max(val, minimum), maximum); }
float _clamp(float val){ return _clamp(val, 0.0, 1.0); }
vec2  _clamp(vec2  val){ return vec2(_clamp(val.r  ), _clamp(val.g)); }
vec3  _clamp(vec3  val){ return vec3(_clamp(val.rg ), _clamp(val.b)); }
float rand(vec2 co){ return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453); }

float left (float val){ return rand(vec2(0, 1)); }
float right(float val){ return rand(vec2(0, 1)); }

uniform int iter = 500;

void main() {
    int i;
    float x, y;
    vec2 p, c, z;

    color = vec4(1);
    p = vec2(0, 0);

    z = p * vec2(1.2, 1.2);
    c = vec2(cos(time/1200.0),sin(time/1000.0));
    for(i=0; i < iter; ++i) {
        x = (z.x * z.x - z.y * z.y) + c.x + rand(c) / 10;
        y = (z.x * z.y + z.x * z.y) + c.y + rand(c) / 10;

        if((x * x + y * y) > 4.0)
            break;
        
        z.x = x;
        z.y = y;
    }
    if(i < iter){
        vec3 nonwhite = _clamp(vec3(
		(float(i) * 5.0 / float(iter) - 0.7) / 0.3,
		(float(i) * 5.0 / float(iter) - 0.3) / 0.7,
		float(i) * 5.0 / float(iter)));
        if(!(nonwhite == vec3(0, 0, 0)))
            color.rgb = nonwhite;
    }
    
    vec2 blur = vec2(rand(uv), rand(uv));

    if(x < 0.1 && y < 0.1)
        color.rgb += texture(AAA, uv + blur * vec2(rand(c), rand(c))).rgb * 0.5;
    else if(x < 0.5 && x > 0.3)
        color.rgb += texture(AAA, uv - vec2(rand(c), rand(c)) * 0.1).rgb * 0.5;
    else if(y < 0.5 && y > 0.3)
        color.rgb += texture(AAA, uv - vec2(rand(c), rand(c)) * 0.01 * blur).rgb * 0.5;
    else
        color.rgb += texture(AAA, uv + vec2(rand(c), rand(c)) * 0.01).rgb * 0.5;
}
