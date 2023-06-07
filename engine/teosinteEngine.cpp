#include "teosinteEngine.hpp"

#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include <fstream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include <glm/gtx/string_cast.hpp>
#include "mesh.h"
#include "model.h"
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

#include <ft2build.h>
#include FT_FREETYPE_H





int SCR_WIDTH  = 1920;
int SCR_HEIGHT = 1080;
float generalscale = 1.1;
const int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
//const unsigned int SCR_WIDTH = 400;
//const unsigned int SCR_HEIGHT = 225;
//const unsigned int SCR_WIDTH = 640;
//const unsigned int SCR_HEIGHT = 360;
bool wireframeEnabled = false;
bool firstMouse = true;

glm::vec3 cameraPos   = glm::vec3(0.0f, 1.5f,  -4.0f);

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float fov   =  45.0f;


float lastFrame = 0.0f; // Time of last frame


float quadVertices[] = 
{ // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};
float skyboxVertices[] = 
{
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };


std::vector<std::string> faces
    {
        "assets/texturas/skybox/right.png",
        "assets/texturas/skybox/left.png",
        "assets/texturas/skybox/top.png",
        "assets/texturas/skybox/bottom.png",
        "assets/texturas/skybox/front.png",
        "assets/texturas/skybox/back.png"
    };

Camera* cam;


//--------------------------------------------------------------------------------------------------//
//------------------------------------ ENGINE GL ---------------------------------------------------//
//--------------------------------------------------------------------------------------------------//
/*
//------------------------------------- CONTAINS ---------------------------------------------------//

 - All Shaders for easy access
 - Utility functions such as
    - Text Drawing
    - Instance Generations
    - Skybox
    - Menus
    - Access to DELTA TIME

//--------------------------------------------------------------------------------------------------//
*/
EngineGL::EngineGL(){
    deltaTime = 0.0f;	// Time between current frame and last frame
    SRC_HEIGHT = SCR_HEIGHT;
    SRC_WIDTH = SCR_WIDTH;
    inventario = Model("assets/level1/background_inventario.obj",glm::vec3( 0.0f,  0.0f,  0.0f));
    inventario.Rotate(90,0,1,0);
    inventario.Rotate(-90,1,0,0);
    transition = Model("assets/level1/background_transition.obj",glm::vec3( 0.0f,  0.0f,  0.0f));
    transition.Rotate(90,0,1,0);
    transition.Rotate(-90,1,0,0);
    plano = Model("assets/level1/textura_prueba.obj",glm::vec3( 0.0f,  0.0f,  0.0f));
    plano.Rotate(90,0,1,0);
    plano.Rotate(-90,1,0,0);
    bgoptions = Model("assets/level1/plano_opciones.obj",glm::vec3( 0.0f,  0.0f,  0.0f));
    bgoptions.Rotate(90,0,1,0);
    bgoptions.Rotate(-90,1,0,0);
    bgpause = Model("assets/level1/plano_pause.obj",glm::vec3( 0.0f,  0.0f,  0.0f));
    bgpause.Rotate(90,0,1,0);
    bgpause.Rotate(-90,1,0,0);
    bgintro = Model("assets/level1/plano_intro.obj",glm::vec3( 0.0f,  0.0f,  0.0f));
    bgintro.Rotate(90,0,1,0);
    bgintro.Rotate(-90,1,0,0);
    bgcredits = Model("assets/level1/plano_creditos.obj",glm::vec3( 0.0f,  0.0f,  0.0f));
    bgcredits.Rotate(90,0,1,0);
    bgcredits.Rotate(-90,1,0,0);
    bggoodending = Model("assets/level1/plano_goodending.obj",glm::vec3( 0.0f,  0.0f,  0.0f));
    bggoodending.Rotate(90,0,1,0);
    bggoodending.Rotate(-90,1,0,0);
    bgbadending = Model("assets/level1/plano_badending.obj",glm::vec3( 0.0f,  0.0f,  0.0f));
    bgbadending.Rotate(90,0,1,0);
    bgbadending.Rotate(-90,1,0,0);
    bgcontrols = Model("assets/level1/plano_controls.obj",glm::vec3( 0.0f,  0.0f,  0.0f));
    bgcontrols.Rotate(90,0,1,0);
    bgcontrols.Rotate(-90,1,0,0);
    
    candilBar = Model("assets/level1/candilbar.obj",glm::vec3( -0.8f,  -0.6f,  0.0f));
    candilBar.Rotate(90,0,1,0);
    candilBar.Rotate(-90,1,0,0);
    candilBar.Scale(0.2,0.2,0.3);
    candilResource = Model("assets/level1/candilResource.obj",glm::vec3( -0.8f,  -0.6f,  0.0f));
    candilResource.Rotate(90,0,1,0);
    candilResource.Rotate(-90,1,0,0);
    candilResource.Scale(0.2,0.2,0.3);
    selector = Model("assets/level1/plano_selector.obj",glm::vec3( -0.8f,  -0.6f,  0.0f));
    selector.Rotate(90,0,1,0);
    selector.Rotate(-90,1,0,0);
    selector.Scale(1,0.13,0.21);
    death = Model("assets/level1/end.obj",glm::vec3( 0.0f,  0.0f,  0.0f));
    death.Rotate(90,0,1,0);
    death.Rotate(-90,1,0,0);

    initshaders();

    initskybox();

    initdepthcubemap();

    camera = Camera(cameraPos);
    initFreetype();

}

EngineGL::~EngineGL(){
    //delete []modelMatrices; 
    //modelMatrices = nullptr; 
}

void EngineGL::initshaders()
{
    shader = Shader("src/engine/shaders/VertexShader.vs","src/engine/shaders/FragmentShader.fs");
    shaderPlayer = Shader("src/engine/shaders/VertexShaderPlayer.vs","src/engine/shaders/FragmentShader.fs");
    instanceshader = Shader("src/engine/shaders/VertexShaderInstances.vs","src/engine/shaders/FragmentShaderInstances.fs");
    lightcubeshader = Shader("src/engine/shaders/VertexShaderLight.vs","src/engine/shaders/FragmentShaderLight.fs");
    Guishader = Shader("src/engine/shaders/VertexShaderGui.vs","src/engine/shaders/FragmentShaderGui.fs");
    skyboxshader = Shader("src/engine/shaders/VertexShaderSkybox.vs","src/engine/shaders/FragmentShaderSkybox.fs");
    textshader = Shader("src/engine/shaders/VertexShaderText.vs","src/engine/shaders/FragmentShaderText.fs");
    simpleDepthShader = Shader("src/engine/shaders/VertexShaderDepth.vs","src/engine/shaders/FragmentShaderDepth.fs","src/engine/shaders/GeometryShaderDepth.gs");
    animationShader = Shader("src/engine/shaders/VertexShaderAnimation.vs","src/engine/shaders/FragmentShader.fs");
    
}

void EngineGL::initskybox()
{
    //inicializacion del skybox
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    this->cubemapTexture = loadCubemap();
    this->skyboxshader.use();
    this->skyboxshader.setInt("skybox",0);
}

void EngineGL::initdepthcubemap()
{
    glGenFramebuffers(1, &depthMapFBO);
    // create depth cubemap texture
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shader.setInt("diffuseTexture", 0);
    shader.setInt("depthMap", 1);
}

int EngineGL::initFreetype()
{
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SRC_WIDTH), 0.0f, static_cast<float>(SRC_HEIGHT));
    textshader.use();
    glUniformMatrix4fv(glGetUniformLocation(textshader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    FT_Library ft;
     // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

	// find path to font
    std::string font_name = ("assets/fonts/palatino.ttf");
    if (font_name.empty())
    {
        std::cout << "ERROR::FREETYPE: Failed to load font " +font_name << std::endl;
        return -1;
    }
	
	// load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    
    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &characterVAO);
    glGenBuffers(1, &characterVBO);
    glBindVertexArray(characterVAO);
    glBindBuffer(GL_ARRAY_BUFFER, characterVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return 0;
}


int EngineGL::ProcessInstanceTXT(const char * myfile,Model model, EngineGL &GL, bool iscorn)
{
    int amount=0;
    //std::cout << myfile << "\n";
    std::ifstream infile(myfile);
    float x,y,z;
    float rx,ry,rz;
    int i = 0;

    while(infile >> x >> y >> z >> rx >> ry >> rz)
    {   
        amount++;
        //std::cout << "NUMERO DE VALLA "<< amount << "X  " << x  << "  Y " << y  << "  Z " << z  << "  RX " << rx  << "  RY " << ry  << "  RZ " << rz << "\n"; 
    }

    //std::cout << "ctd de vallas" << amount << "\n";
    //Esto da memory leak
    glm::mat4 amountt[amount];
    GL.modelMatrices = amountt;
    std::ifstream infile2(myfile);

    if(iscorn)
    {
        while(infile2 >> x >> z >> y >> rx >> ry >> rz) 
        {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-x, y, z));
        model = glm::rotate(model,  rx, glm::vec3(1,0,0));
        model = glm::rotate(model,  ry, glm::vec3(0,1,0)); 
        model = glm::rotate(model,  rz, glm::vec3(0,0,1));
        
        float size = 0.7 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.3-0.7)));
        model = glm::scale(model,glm::vec3(1.2,size,1.2));
        modelMatrices[i] = model;
        i++;
        }
    }
    else
    {
        while(infile2 >> x >> z >> y >> rx >> ry >> rz) 
        {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-x, y, z));
        model = glm::rotate(model,  rx, glm::vec3(1,0,0));
        model = glm::rotate(model,  ry, glm::vec3(0,1,0)); 
        model = glm::rotate(model,  rz, glm::vec3(0,0,1));
        
        //double f = rand() % 1;
        //f += 0.5;
        //std::cout << "aaa" << f << "\n";
        //model = glm::scale(model, glm::vec3(1,f,1));
        
        modelMatrices[i] = model;
        i++;
        }
    }
    

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &GL.modelMatrices[0], GL_STATIC_DRAW);

    for(unsigned int i = 0; i < model.meshes.size(); i++)
    {
        unsigned int VAO = model.meshes[i].VAO;
        glBindVertexArray(VAO);
        // vertexbackpack attributes
        std::size_t vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3); 
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
        glEnableVertexAttribArray(4); 
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
        glEnableVertexAttribArray(5); 
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6); 
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }  

    return amount;
}

void EngineGL::renderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state	
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    textshader.use();
    glUniform3f(glGetUniformLocation(textshader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(characterVAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = Characters[*c];

        float xpos = x - 110 + ch.Bearing.x * scale * generalscale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale * generalscale;

        float w = ch.Size.x * scale * generalscale;
        float h = ch.Size.y * scale * generalscale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, characterVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale * generalscale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
}

void EngineGL::renderMultipleText(std::vector<std::string> texts)
{
    for(uint8_t i=0; i < texts.size(); i++) 
    {
        renderText(texts[i], 120.0f - 10*i, 120.0f - 10*i, 0.35f, glm::vec3(1.0f, 1.0f, 1.0f));
    }
}





//--------------------------------------------------------------------------------------------------//
//------------------------------------ FRAMEBUFFER -------------------------------------------------//
//--------------------------------------------------------------------------------------------------//
/*
//------------------------------------- CONTAINS ---------------------------------------------------//

 - Framebuffer inicialization
 - Clear function
 - Change function
 - Depth Map for Shadows


//--------------------------------------------------------------------------------------------------//
*/

framebuffer::framebuffer()
{   

    unsigned int quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    
    framebuffershader.use();
    framebuffershader.setInt("screenTexture",0);

    //attach one buffer, attach color, reserve memory, use!
    //attach buffer
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo); //binding the next read and write operations

    //attach color (texture) (la generamos, nos la inventamos)
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); //generamos textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//sus params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
    //aqui lo juntamos todo
    //framebuffer a targetear,attachment, textura target, el puntero, y el nivel mipmap (no se que es eso)
    //bindeamos 

    //los render buffer objects RBOs son posbiles attachments del FBO, son buffers de cosas bytes ints pixeles etc y no se pueden leer
    //los usamos porque aunque no se pueda leer directamente es mas rapido para escribir
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    //le metemos stencil y depth
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    //y attacheamos
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "se ha jodido el framebuffer" << "\n";
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    framebuffershader = Shader("src/engine/shaders/VertexShaderFramebuffer.vs","src/engine/shaders/FragmentShaderFramebuffer.fs");
    framebuffershader.use();
    framebuffershader.setInt("screenTexture",0);

    framebuffershader.setInt("hp",1);

    isTheCurrentMain = false;
    
}


void framebuffer::changeFramebuffer()
{
    if(isTheCurrentMain)
    {
        glBindFramebuffer(GL_FRAMEBUFFER,fbo);
    }
    else 
    {
        glBindFramebuffer(GL_FRAMEBUFFER,0);
    }

}


void framebuffer::clearbuffer() //cuando acabemos las operaciones siempre llamar
{
    glDeleteFramebuffers(1,&fbo);
}



//--------------------------------------------------------------------------------------------------//
//------------------------------------TEOSINTE ENGINE ----------------------------------------------//
//--------------------------------------------------------------------------------------------------//
/*
//------------------------------------- CONTAINS ---------------------------------------------------//

Disclaimer: nothing besides the game loop should have access to these functions. DO NOT USE.
 - Initialization of all OpenGL variables
 - Init and stop functions for Mouse and Keyboard
 - Update and render functions for all the shaders involved
 - Cubemap Loader


//--------------------------------------------------------------------------------------------------//
*/

GLFWwindow* startEngine()
{
    glfwInit(); //Inicializamos GLFW

    //y procedemos a darle informacion a la ventana, (el que y a que)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //de la version de opengl... 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //access to smaller subset of OPENGL features
    
    //if MACOS opengl_foward_compat, gl true

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Teosinte engine", glfwGetPrimaryMonitor() ,NULL); //glfwGetPrimaryMonitor()
 

    if(window==NULL)
    {
        std::cout << "Failed to create Teosinte Window" << std::endl;
        glfwTerminate();
        return nullptr;
    } 
    glfwMakeContextCurrent(window);
    
    
    
    //tras eso iniciamos GLAD que maneja punteros de OPENGL ((muy importante!!!))

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD, no pointers for u!" << std::endl;
        return nullptr;
    }

    stbi_set_flip_vertically_on_load(true);
    //stbi_set_flip_vertically_on_load_thread(true);
    //glEnable(GL_DEPTH_TEST);
      //glEnable(GL_CULL_FACE);
    

    //tenemos que decirle a opengl el tamaño de la ventana para que muestre cosas en base a la ventana
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //PONER A                             NORMAL PARA USAR IMGUI
                                        //DISABLED PARA JUGAR NORMAL
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    if(wireframeEnabled)
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    glDisable(GL_DITHER);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH, GL_DONT_CARE);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
    #define GL_MULTISAMPLE_ARB 0x809D
    glDisable( GL_MULTISAMPLE_ARB);
    return window;


}


void updateGlobals(EngineGL& GL)
{
    SCR_HEIGHT = GL.SRC_HEIGHT;
    SCR_WIDTH =  GL.SRC_WIDTH;
    
    switch(GL.SRC_HEIGHT)
    {
        case 1080: //1080x1920
        {   
            generalscale=1.1;
            break;
        }
        case 450:
        { 
            generalscale=0.58;
            break;
        }
        case 270:
        { 
            generalscale=0.38;
            break;
        }
        default:
        {
            generalscale=1;
        }
    }
    GL.initFreetype(); 

    //std::cout << "src height GL " << GL.SRC_HEIGHT << "src width GL  " << GL.SRC_WIDTH << "\n";
    //std::cout << "src height    " << SCR_HEIGHT << "src width        " << SCR_WIDTH << "\n" << "\n";
    
}

void initRaton(GLFWwindow* window){
    glfwSetCursorPosCallback(window, mouse_callback);
}

void UpdateOpenGL(GLFWwindow* window, EngineGL& GL, framebuffer frameB)
{
    cam = &GL.camera;
    float currentFrame = glfwGetTime();
    GL.deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;  
    //inputs
   // processInput(window, man_e);
    glBindFramebuffer(GL_FRAMEBUFFER,frameB.fbo);
    //rendering 


    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /*
    //si queremos cambiar info en shaders, usamos uniforms TRAS ACTIVAR EL SHADER, para llamarlos desde su nom de variable y manipularlos, ejemplo
    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    //int vertexColorLocation = glGetUniformLocation(GL.shader.ID, "ourColor");
    //glUniform4f(vertexColorLocation, greenValue, greenValue, greenValue, 1.0f); //lo actualizamos, de esto hay setters de valores en shader.h
    GL.shader.setFloat("posHor",greenValue); */
    GL.shader.use();

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(ZOOM), (float)GL.SRC_WIDTH/ GL.SRC_HEIGHT, 0.1f, 100.0f); // siendo 800 y 600 el tam de la camara

    //CAMARA  
    glm::mat4 view;
    view = GL.camera.GetViewMatrix();

    GL.shader.setMat4("view",view);
    GL.shader.setMat4("projection",projection);
    
    GL.shader.setVec3("objectColor", 0.1f, 0.1f, 0.1f);

    GL.shader.setVec3("viewerPos", GL.camera.Position);
    

     //GL.shader.use();
    GL.shader.setInt("material.diffuse", 0);
    GL.shader.setInt("material.specular", 1);

    GL.shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    GL.shader.setFloat("material.shininess", 32.0f);



  

    // directional light
    GL.shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    GL.shader.setVec3("dirLight.ambient", 0.20f, 0.2f, 0.2f);
    GL.shader.setVec3("dirLight.diffuse", 0.12f, 0.12f, 0.12f);
    GL.shader.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);


    GL.animationShader.use();
    GL.animationShader.setMat4("view",view);
    GL.animationShader.setMat4("projection",projection);
    GL.animationShader.setVec3("objectColor", 0.1f, 0.1f, 0.1f);
    GL.animationShader.setVec3("viewerPos", GL.camera.Position);
    GL.animationShader.setInt("material.diffuse", 0);
    GL.animationShader.setInt("material.specular", 1);
    GL.animationShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    GL.animationShader.setFloat("material.shininess", 32.0f);
    GL.animationShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    GL.animationShader.setVec3("dirLight.ambient", 0.20f, 0.2f, 0.2f);
    GL.animationShader.setVec3("dirLight.diffuse", 0.12f, 0.12f, 0.12f);
    GL.animationShader.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);
    
}

void UpdateShaderPlayer(EngineGL& GL){


    GL.shaderPlayer.use();

    //GL.shaderPlayer.setVec3("objectColor", 0.1f, 0.1f, 0.1f);

    GL.shaderPlayer.setInt("material.diffuse", 0);
    GL.shaderPlayer.setInt("material.specular", 1);

    GL.shaderPlayer.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    GL.shaderPlayer.setFloat("material.shininess", 32.0f);

    GL.shaderPlayer.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    GL.shaderPlayer.setVec3("dirLight.ambient", 0.0001f, 0.0001f, 0.0001f);
    GL.shaderPlayer.setVec3("dirLight.diffuse", 0.05f, 0.05f, 0.05f);
    GL.shaderPlayer.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);
}




void renderLights(GLFWwindow* window, EngineGL& GL)
{
    /*
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(ZOOM), 800.0f / 600.0f, 0.1f, 100.0f); // siendo 800 y 600 el tam de la camara

    //CAMARA  
    glm::mat4 view;
    view = GL.camera.GetViewMatrix(); */
    
}   




void renderGui(EngineGL& GL)
{
    if(GL.firstLanternHeld)
    {

        GL.Guishader.use();

        GL.candilBar.Draw(GL.Guishader);

        GL.candilResource.Draw(GL.Guishader);
    }    

}


void renderinstances(EngineGL& GL, Manentity_type& man_e){
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(ZOOM), (float)GL.SRC_WIDTH/ GL.SRC_HEIGHT, 0.1f, 100.0f); // siendo 800 y 600 el tam de la camara
    glm::mat4 view;
    view = GL.camera.GetViewMatrix();
    GL.instanceshader.use();
    GL.instanceshader.setMat4("view",view);
    GL.instanceshader.setMat4("projection",projection);
    GL.instanceshader.setVec3("viewerPos", GL.camera.Position);

    GL.instanceshader.setVec3("objectColor", 0.1f, 0.1f, 0.1f);
    GL.instanceshader.setInt("material.diffuse", 0);
    GL.instanceshader.setInt("material.specular", 1);
    GL.instanceshader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    GL.instanceshader.setFloat("material.shininess", 32.0f);
    GL.instanceshader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    GL.instanceshader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
    GL.instanceshader.setVec3("dirLight.diffuse", 0.05f, 0.05f, 0.05f);
    GL.instanceshader.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);

    auto& cPhy = man_e.getEntityCMP<CPhysics>(man_e.getEntityArray()[2]);
    auto& cPL = man_e.getEntityCMP<CPointLight>(man_e.getEntityArray()[2]);
    GL.instanceshader.setVec3("pointLights[0].position", cPhy.x,cPhy.y,cPhy.z);
    GL.instanceshader.setVec3("pointLights[0].ambient", cPL.ambientx, cPL.ambienty, cPL.ambientx);
    GL.instanceshader.setVec3("pointLights[0].diffuse", cPL.diffusex, cPL.diffusey, cPL.diffusex);
    GL.instanceshader.setVec3("pointLights[0].specular", cPL.specularx, cPL.speculary, cPL.specularx);
    GL.instanceshader.setFloat("pointLights[0].constant", cPL.constant);
    GL.instanceshader.setFloat("pointLights[0].linear", cPL.linear);
    GL.instanceshader.setFloat("pointLights[0].quadratic", cPL.quadratic);
}

void renderFramebuffer(framebuffer frameB)
{

    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    frameB.framebuffershader.use();
    float timeValue = glfwGetTime();

    frameB.framebuffershader.setFloat("iTime",timeValue);
    glBindVertexArray(frameB.quadVAO);
    glBindTexture(GL_TEXTURE_2D, frameB.textureColorBuffer);
    glDrawArrays(GL_TRIANGLES,0,6);

}




// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap()
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}




void renderskybox(EngineGL &GL)
{
    // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        GL.skyboxshader.use();
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(ZOOM), (float)GL.SRC_WIDTH/ GL.SRC_HEIGHT, 0.1f, 100.0f); // siendo 800 y 600 el tam de la camara
        glm::mat4 view;
        view = glm::mat4(glm::mat3(GL.camera.GetViewMatrix()));
        GL.skyboxshader.setMat4("view",view);
        GL.skyboxshader.setMat4("projection",projection);


        glBindVertexArray(GL.skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, GL.cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
}

void afterRenderUpdatesOpenGL(GLFWwindow* window)
{
    //glfwSwapBuffers(window); //doble buffer como en amstrad
    //glfwPollEvents(); //procesa eventos en cola, includo teclas, actualizaciones de ventan
}


void closeOpenGL()
{
    glfwTerminate();
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void mouse_callback(GLFWwindow* window, double posx, double posy)
{
    float xpos = static_cast<float>(posx);
    float ypos = static_cast<float>(posy);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    
    cam->ProcessMouseMovement(xoffset, yoffset);
    
}

void mouse_menu_callback(GLFWwindow* window, double posx, double posy)
{   
    


}