#include "CPointLight.hpp"


/* QUADRATIC VALUES
Distance 	Constant 	Linear 	Quadratic
7 	        1.0 	     0.7 	1.8
13 	        1.0 	     0.35 	0.44
20 	        1.0 	     0.22 	0.20
32 	        1.0 	     0.14 	0.07
50 	        1.0 	     0.09 	0.032
65 	        1.0 	     0.07 	0.017
100 	    1.0 	    0.045 	0.0075
160 	    1.0 	    0.027 	0.0028
200 	    1.0 	    0.022 	0.0019
325 	    1.0 	    0.014 	0.0007
600 	    1.0 	    0.007 	0.0002
3250 	    1.0 	    0.0014 	0.000007


*/




void CPointLight::setOff()
{
    ambientx  = 0.5;
    ambienty  = 0.5;
    ambientz  = 0.5;
    diffusex  = 1.0;
    diffusey  = 1.0;
    diffusez  = 1.0;
    specularx = 1.0;
    speculary = 1.0;
    specularz = 1.0;
    quadratic = 1.8;
    linear = 0.7;
    isOn = false;
}

void CPointLight::setOnLamp()
{
    ambientx  = 2.2;
    ambienty  = 2.2;
    ambientz  = 2.2;
    diffusex  = 0.9;
    diffusey  = 0.9;
    diffusez  = 0.9;
    specularx = 0.4;
    speculary = 0.4;
    specularz = 0.4;
    quadratic = 0.14;
    linear = 0.07;
    isOn=true;
}


void CPointLight::debugLight()
{
    ambientx  = 2;
    ambienty  = 1;
    ambientz  = 1;
    diffusex  = 0.9;
    diffusey  = 0.9;
    diffusez  = 0.9;
    specularx = 0.4;
    speculary = 0.4;
    specularz = 0.4;
    quadratic = 0.35;
    linear = 0.44;
    isOn=true;
}



void CPointLight::bigLight()
{
    ambientx  = 1.2;
    ambienty  = 2;
    ambientz  = 1.7;
    diffusex  = 0.6;
    diffusey  = 1.0;
    diffusez  = 0.85;
    specularx = 0.3;
    speculary = 0.5;
    specularz = 0.43;
    quadratic = 0.2;
    linear = 0.22;
    isOn=true;
}

void CPointLight::smallLight()
{
    ambientx  = 1.2;
    ambienty  = 2;
    ambientz  = 1.7;
    diffusex  = 0.6;
    diffusey  = 1.0;
    diffusez  = 0.85;
    specularx = 0.3;
    speculary = 0.5;
    specularz = 0.43;
    quadratic = 1.8;
    linear = 0.7;
    isOn=true;
}

void CPointLight::houseLight()
{
    ambientx  = 1.2;
    ambienty  = 1.2;
    ambientz  = 1.2;
    diffusex  = 0.6;
    diffusey  = 0.6;
    diffusez  = 0.6;
    specularx = 0.3;
    speculary = 0.3;
    specularz = 0.3;
    quadratic = 0.22;
    linear = 0.20;
    isOn=true;
}