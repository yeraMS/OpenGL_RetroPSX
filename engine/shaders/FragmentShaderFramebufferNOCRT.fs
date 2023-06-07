#version 330 core


out vec4 FragColor;

in vec2 screenCoord;


uniform sampler2D screenTexture;
uniform sampler2D noisetexture;

uniform float hp;



void main()
{   

	//FragColor = vec4(FragColor.r + (1-hp), FragColor.gb * hp + (0.1 * (1-hp)),1.0);
	FragColor = texture(screenTexture, screenCoord);
	FragColor = vec4(FragColor.r + (1-hp), FragColor.gb * hp + (0.1 * (1-hp)),1.0);
	
}