#version 150

#define     PI      3.14159265358979323
#define TWO_PI      6.28318530718
#define   PI_8      0.3926990817

// ==== uniforms ==== //
uniform float               u_time;
uniform vec2                u_resImg;
uniform vec2                u_offset;


in vec2 texCoordVarying;

out vec4 outputColor;

// test colors
vec3 magenta = vec3(1.0, .3, .6);
vec3 avocado = vec3(0.34, .51, .01);
vec3 turquoise = vec3(0.1, .21, .99);
vec3 orchid = vec3(.85, 0.44, .84);
vec3 yellow4 = vec3(1, 1, .2);
vec3 fluoGreen = vec3( .8, 1, 0);

float Hash(vec2 p){
    p = fract(p * vec2( 123.34, 456.21));
    p += dot(p, p + 5.32);
    return fract(p.x*p.y);
}

// coming from :
// https://thebookofshaders.com/11/
// &&
// https://www.shadertoy.com/view/4dS3Wd

float noise(vec2 st) {
    
    st += u_time * 0.1;
    
    vec2 i = floor(st);
    vec2 f = fract(st);
    vec2 u = f*f*(3.0-2.0*f);
    return mix( mix( Hash( i + vec2(0.0,0.0) ),
                     Hash( i + vec2(1.0,0.0) ), u.x),
                mix( Hash( i + vec2(0.0,1.0) ),
                     Hash( i + vec2(1.0,1.0) ), u.x), u.y);
}

mat2 rotate2d(float angle){
    return mat2(cos(angle),-sin(angle),
                sin(angle),cos(angle));
}


float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                * 43758.5453123);
}


// specs
float imperfections(vec2 uv, float impurityAmnt, float size){
    
    uv *= size;
    
    uv += noise(uv * 2.);
    uv -= noise(uv * .1);
    
    return 1 - smoothstep(.0,impurityAmnt, noise(uv));
}

float outlineNoise(vec2 uv, float n){
    
    float outline = 0;
    
    uv = .3 * rotate2d( .5 * noise(uv) ) * uv;
    outline += .6 * (1. - smoothstep(0, n, abs(sin(uv.x * TWO_PI))));
    
    return outline;
}

float layer(vec2 uv, float startVal, float coeffSmooth, float coeffNoise){
    float l = 0;
    
    float t = smoothstep(startVal, 1. - coeffSmooth * noise(uv), noise(uv));
    l += coeffNoise * t;
    
    l +=  outlineNoise(uv, .1 * t);
    
    return l;
}


vec3 globalNoise(vec2 uv){
    
    vec3 n = vec3(0);
    
    
    uv *= 8 + cos(u_time * 0.01);
    
    uv = rotate2d(PI / 6) * uv;
    uv -= noise(uv * .6);
    float noiseTexel = noise(uv);
    
    n = vec3( .4 * (1 - smoothstep(0, .5, noiseTexel))) * vec3(noiseTexel * turquoise.x, noiseTexel + turquoise.y, turquoise.z);
    
    

    uv += .3 * rotate2d(noise(uv)) * uv;
    n +=  .1 * (1 - smoothstep(.4, 1., noise(uv)));
    
    noiseTexel = noise(uv);
    
    n += layer(uv, .4, .4, .6) * vec3(yellow4.x, yellow4.y * noiseTexel, yellow4.z);
    n += layer(uv, -.1, .2, .1) * magenta;
    
    float s = sin(u_time * .4) * .3;
    uv += .1 * rotate2d(noise(uv) + PI_8) * uv;
    n += .5 * layer(uv, s + .3, .65, .5 * cos(u_time * .4)) * magenta;

    n -= .6 * smoothstep(.6, 1. - .2 * noiseTexel, noiseTexel) * turquoise;
    n += .03 * smoothstep(.96, 1. - .06 * noiseTexel, noiseTexel) * vec3(orchid.r, orchid.g * noiseTexel, orchid.b * noiseTexel);
    n += .02 * smoothstep(.2, .2 + .4 * noiseTexel, noiseTexel) * magenta;
    
    return n;
}




void main( void )
{
    vec2 uv = texCoordVarying;
    uv /= u_resImg;
//    uv.x *= u_resImg.x / u_resImg.y;
    
    orchid += vec3(.3 * cos(u_time* .2), 0,0);
    turquoise += vec3(0, .5 + .5 * cos(u_time * .2), .5 + .5 * sin(u_time * .3));
    magenta.r += .4 * sin(u_time* .4);
    magenta.b += .4 * cos(u_time* .6);
    yellow4 += vec3(.3 * sin(u_time * .03), .15 * sin(u_time * .03), 0);
    
    
    uv += u_offset;
    
    vec3 color = globalNoise( uv );
    
    float impurityAmnt = .02;
    float size = 50;
    color += imperfections(uv, impurityAmnt, size)* orchid + imperfections(rotate2d(PI) * uv, impurityAmnt * 1.5,  size) * turquoise;
    color += .04 + .04 * Hash( uv )* fluoGreen * sin(u_time* .1 + PI);
    
    
    outputColor = vec4(color, 1.0);
}

