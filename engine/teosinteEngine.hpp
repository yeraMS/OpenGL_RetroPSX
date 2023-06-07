//g++ main.cpp glad.c -ldl -lglfw && ./a.out 

#pragma once
#include <glm/glm.hpp> //acordaos de trizen GLM 
#include "shader.h"
#include "model.h"
#include "camera.h"
//VARIABLES GLOBALES 
// settings


struct EngineGL
{
    struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
    };


    float deltaTime;	// Time between current frame and last frame

    unsigned int skyboxVAO, skyboxVBO, cubemapTexture;

    unsigned int characterVAO, characterVBO;
    std::map<GLchar, Character> Characters;



    unsigned int depthMapFBO;
    unsigned int depthCubemap;


    Shader shader;
    Shader shaderPlayer;
    Shader Guishader;
    Shader instanceshader;
    Shader lightcubeshader;
    Shader skyboxshader;
    Shader textshader;
    Shader simpleDepthShader;
    Shader animationShader;

    //planos de fondo con shader GUI
    Model plano;
    Model bgoptions;
    Model inventario;
    Model transition;
    Model bgpause;
    Model death;
    Model bgintro;
    Model bgcredits;
    Model bgbadending;
    Model bggoodending;
    Model bgcontrols;

    // otras cosas utiles
    Model instance;
    Model candilBar;
    Model candilResource;
    Model selector;
    Camera camera;
    unsigned int SRC_WIDTH;
    unsigned int SRC_HEIGHT;
    int final_{0};

    //para el primer lv y codelock cortar renderizaciones(un poco cutre)
    bool firstLanternHeld;
    bool incodelock;
    bool codelock_completed = false;

    inline static std::size_t nextIDLight { 0 };
    glm::mat4* modelMatrices;

    EngineGL();

    ~EngineGL();
    
    void changeResolution();
    int ProcessInstanceTXT(const char * myfile,Model model, EngineGL &GL,bool iscorn);

    void initshaders();
    void initskybox();
    void initdepthcubemap();
    int initFreetype();
    void renderText(std::string text, float x, float y, float scale, glm::vec3 color);
    void renderMultipleText(std::vector<std::string> texts);
};


struct framebuffer //offscreen rendering aka post procesado, funciona en otro buffer de memoria no en el framebuffer main de ventana
{
    bool isTheCurrentMain;
    unsigned int fbo;
    unsigned int rbo;
    unsigned int quadVAO;
    unsigned int textureColorBuffer;
    Shader framebuffershader;


    framebuffer();

    void changeFramebuffer();


    void clearbuffer();

};

struct GLFWwindow;
GLFWwindow* startEngine();

void initRaton(GLFWwindow* window);

unsigned int loadCubemap();

void UpdateOpenGL(GLFWwindow* window, EngineGL& GL, framebuffer frameB);

void UpdateShaderPlayer(EngineGL& GL);






void renderLights(GLFWwindow* window, EngineGL& GL);

void renderGui(EngineGL& GL);

void renderinstances(EngineGL& GL, Manentity_type& man_e);

void renderFramebuffer(framebuffer frameB);

void renderskybox(EngineGL& GL);

void afterRenderUpdatesOpenGL(GLFWwindow* window);


void updateGlobals(EngineGL& GL);

void closeOpenGL();


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void mouse_callback(GLFWwindow* window, double posx, double posy);
void mouse_menu_callback(GLFWwindow* window, double posx, double posy);
