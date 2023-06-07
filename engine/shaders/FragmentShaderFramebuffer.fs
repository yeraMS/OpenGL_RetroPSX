#version 330 core


out vec4 FragColor;

in vec2 screenCoord;


uniform sampler2D screenTexture;
uniform sampler2D noisetexture;

uniform float iTime;
uniform float hp;
const float scan = 0.95;




float scanline(vec2 uv) {
	return sin(320 * uv.y * 0.7 - iTime * 10.0);
}

float slowscan(vec2 uv) {
	return sin(320 * uv.y * 0.02 + iTime * 6.0);
}

vec2 colorShift(vec2 uv) {
	return vec2(
		uv.x,
		uv.y + sin(iTime)*0.02
	);
}

float noise(vec2 uv) {
	return clamp(texture(screenTexture, uv.xy + iTime*6.0).r +
		texture(screenTexture, uv.xy - iTime*4.0).g, 0.96, 1.0);
}

// from https://www.shadertoy.com/view/4sf3Dr
// Thanks, Jasper
vec2 crt(vec2 coord, float bend)
{
	// put in symmetrical coords
	coord = (coord - 0.5) * 2.0;

	coord *= 0.5;	
	
	// deform coords
	coord.x *= 1.0 + pow((abs(coord.y) / bend), 2.0);
	coord.y *= 1.0 + pow((abs(coord.x) / bend), 2.0);

	// transform back to 0.0 - 1.0 space
	coord  = (coord / 1.0) + 0.5;

	return coord;
}

vec2 colorshift(vec2 uv, float amount, float rand) {
	
	return vec2(
		uv.x,
		uv.y + amount * rand // * sin(uv.y * iResolution.y * 0.12 + iTime)
	);
}

vec2 scandistort(vec2 uv) {
	float scan1 = clamp(cos(uv.y * 2.0 + iTime), 0.0, 1.0);
	float scan2 = clamp(cos(uv.y * 2.0 + iTime + 4.0) * 10.0, 0.0, 1.0) ;
	float amount = scan1 * scan2 * uv.x; 
	
	uv.x -= 0.05 * mix(texture(screenTexture, vec2(uv.x, amount)).r * amount, amount, 0.9);

	return uv;
}

float vignette(vec2 uv) {
	uv = (uv - 0.5) * 0.98;
	return clamp(pow(cos(uv.x * 3.1415), 1.2) * pow(cos(uv.y * 3.1415), 1.2) * 50.0, 0.0, 1.0);
}

void main()
{   
     //BASICO
    FragColor = texture(screenTexture, screenCoord);
     //COLOR INVERTIDO
    FragColor = vec4(vec3(1.0-texture(screenTexture, screenCoord)), 1.0);
     //BLANCO Y NEGRO
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    float hpdone = clamp(hp,average,1.0);
    FragColor = vec4(hpdone, hpdone, hpdone, 1.0);
     //squared distance from center



	//OTRA CRT
    vec2 uv = screenCoord;
	vec2 sd_uv = scandistort(uv);
	vec2 crt_uv = crt(sd_uv, 1.8);
	
	vec4 color;
	
	//float rand_r = sin(iTime * 3.0 + sin(iTime)) * sin(iTime * 0.2);
	//float rand_g = clamp(sin(iTime * 1.52 * uv.y + sin(iTime)) * sin(iTime* 1.2), 0.0, 1.0);
	vec4 rand = texture(screenTexture, vec2(iTime * 0.01, iTime * 0.02));
	
	color.r = texture(screenTexture, crt(colorshift(sd_uv, 0.025, rand.r), 2.0)).r;
	color.g = texture(screenTexture, crt(colorshift(sd_uv, 0.01, rand.g), 2.0)).g;
	color.b = texture(screenTexture, crt(colorshift(sd_uv, 0.024, rand.b), 2.0)).b;	
		
	vec4 scanline_color = vec4(scanline(crt_uv));
	vec4 slowscan_color = vec4(slowscan(crt_uv));

	// Set colour in corners to black
	if (crt_uv.x < 0.0 || crt_uv.x > 1.0)
		color *= 0.0;
	if (crt_uv.y < 0.0 || crt_uv.y > 1.0)
		color *= 0.0;
	
	
	FragColor = mix(color, mix(scanline_color, slowscan_color, 0.5), 0.05) *
		vignette(uv);

	//COLORINES VIDA
	// pone la pantalla casi en negro
	//FragColor = vec4(FragColor.rgb * hp,1.0);
	// no sube tan de golpe como el basico, pero se queda grisecito
    //FragColor = vec4(FragColor.rgb * (hp) + (0.1 * (1-hp)),1.0);
	//test de colores random
	FragColor = vec4(FragColor.r + (1-hp), FragColor.gb * hp + (0.1 * (1-hp)),1.0);
	// A esto se le pueden hacer multiplicaciones super fancy y dificiles para que las cosas del fondo salgan mas rojas/BW o para los bordes pero NPI ahora mismo
	
	//fragColor = vec4(vignette(uv));
	//vec2 scan_dist = scandistort(uv);
	//fragColor = vec4(scan_dist.x, scan_dist.y,0.0, 1.0); 
}