#pragma once
#include <stack>
#include "state.hpp"
#include "../../Game.hpp"
#include "../../engine/teosinteEngine.hpp"

#include "../../Levels.hpp"
#include "../../myImGui.hpp"
#include "../utils/types.hpp"

enum state_id
{
	GAME,
    PREINTRO,
	INTRO,
	MENU,
	OPTIONS,
    DEATH,
    PAUSE,
    CODELOCK,
    FINAL
};

class St_Manager{
    
    public:
    St_Manager(GLFWwindow* window, uint8_t time, uint8_t iatime);
    std::stack<state> states;
    state newState;
    bool isRemoving;    
    bool isAdding;
    bool isReplacing;
    uint8_t carga;
    //OPCIONES
    bool options;
    bool shader;
    bool ison;
    unsigned int image{};
    unsigned int bgoptions{};

    
    Manentity_type GE{};
    EngineGL GL {};
    Game game{};
    Levels lvls{};
	framebuffer frameB{};
    framebuffer lowResFB{};
    std::size_t currentselected;
    void addState(state_id STID, bool isreplacing,GLFWwindow* window);
    void RemoveState();
    void ProcessStateChanges();
    void UpdateEngine(GLFWwindow* window, bool iaupdate);
    void loadmenus(GLFWwindow* window, bool isgamerunning);

    ImTextureID boton1,boton2,boton3, boton4,boton5;
    //lo que antes eran globales cutronas
    int codelock1,codelock2,codelock3,codelock4;
    bool firstgrab,firsttimemenu,credits,controls;
    float x,y,soundvalue,fade;
    int posx,posy,offsetx,offsety;
    
    myImGui GUI;
    

    private: 
        unsigned int getTexture(const char *path);

};