# Into_the_Crops Retro renderer
These are some code snippets of the retro PSX engine, state manager and its integration. Developed in OpenGL, C++ and GLSL.

I decided to keep just some code snippets for the sake of clarity, if you want to check the whole repository just ask me no problem 

GAME HPP AND CPP - our basic game loop calling various render functions as well as sound, collision and other functions through the ECS.

CMP - some code snippets of the components related to the graphical engine used in the ECS.

ENGINE - The engine. Consists of the basic architecture in C++ for cameras,model, texture and shader reading and loading; lights,a few framebuffer and UI shaders, instancing, polygon jittering, cubemaps, text rendering, bone animations, frustum culling and imGui integration.

MAN - Folder with the State Manager. In the complete repository it also includes the whole ECS.

SYS - Some code snippers of the systems used in the ECS.




