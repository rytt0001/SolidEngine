#version 450

in vec2 vUV;
in vec3 vWorldPos;
in vec3 vNormal;
in float time;

uniform vec2 resolution;
uniform vec3 SColor;

#define TAU 6.28318530718
#define MAX_ITER 4
//#define SHOW_TILING

void main( void ) {


	float t = time * 0.8;
    // uv should be the 0-1 uv of texture...
	vec2 uv = gl_FragCoord.xy / resolution.xy;


#ifdef SHOW_TILING
	vec2 p = mod(uv*TAU*2.0, TAU)-250.0;
#else
    vec2 p = mod(uv*TAU, TAU)-250.0;
#endif
	vec2 i = vec2(p);
	float c = 1.0;
	float inten = .005;

	for (int n = 1; n < MAX_ITER; n++)
	{
		float t = t * (1.0 - (3.5 / float(n+1)));
		i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
		c += 1.0/length(vec2(p.x / (sin(i.x+t)/inten),p.y / (cos(i.y+t)/inten)));
	}
	c /= float(MAX_ITER);
	c = 1.17-pow(c, 1.4);
	vec3 colour = vec3(pow(abs(c), 8.0));
    	colour = clamp(colour + SColor, 0.0, 1.0);

#ifdef SHOW_TILING
	// Flash tile borders...
	vec2 pixel = 2.0 / resolution.xy;
	uv *= 2.0;

	float f = floor(mod(time*.5, 2.0)); 	// Flash value.
	vec2 first = step(pixel, uv) * f;		   	// Rule out first screen pixels and flash.
	uv  = step(fract(uv), pixel);				// Add one line of pixels per tile.
	colour = mix(colour, vec3(1.0, 1.0, 0.0), (uv.x + uv.y) * first.x * first.y); // Yellow line

#endif

	gl_FragColor = vec4(colour, 1.0);
}




































