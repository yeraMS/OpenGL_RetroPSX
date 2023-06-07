#include "St_Manager.hpp"
#include "../../engine/model.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

#include "../utils/Timer.hpp"


St_Manager::St_Manager(GLFWwindow* window, uint8_t time, uint8_t iatime)
{   
    //pasar a state game (no todo!)
    currentselected  = 0;
    this->isRemoving = false;
    GE.setTime(time);
    GE.setIATime(iatime);
    GUI.init(window);
    options = false;
    controls = false;
    shader  = true;
    ison = true;
    image = Model::loadImageFromFile("../texturas/start_menu.png", "assets/level1");

    bgoptions = Model::loadImageFromFile("../texturas/start_menu.png", "assets/level1");
    carga = 0;
    GL.firstLanternHeld = false;
    GL.incodelock = false;
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.f, 1.f, 1.f, 0.1f));


    codelock1 = 0;
    codelock2 = 0;
    codelock3 = 0;
    codelock4 = 0;
    firstgrab = true;
    firsttimemenu = true;
    credits = false;
    x = 0;
    y = 0;
    soundvalue = 50;
    fade = 0;

}

void St_Manager::addState(state_id STID, bool isreplacing,GLFWwindow* window)
{
    this->isAdding=true;
    this->isReplacing=isreplacing;

    switch(STID)
    {
        case GAME:
        {
            lvls.emptyAll(GE, game.systems, GL);
            game.systems.soundito.stopAll();
            game.systems.soundito.stopOne("event:/Music/End");
            game.systems.soundito.update();
            lvls.load_lvl_1(GE, game.systems, GL);
            GL.firstLanternHeld = false; //PONED ESTO A TRUE SI NO VAIS A CARGAR EL LV 1
            game.systems.coll_sys.initCollisions(GE);
            game.systems.rend_sys.initinstances(GE, GL);
            newState  = state(state_id::GAME);
            firstgrab = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            initRaton(window);
            break; 
        }

        case PREINTRO:
        {
            newState = state(state_id::PREINTRO);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            ison=true;
            break;
        }

        case INTRO:
        {
            game.systems.soundito.stopAll();
            game.systems.soundito.playSound("event:/Music/MainTheme");
            game.systems.soundito.update();
            newState = state(state_id::INTRO);
            boton1 = (ImTextureID)getTexture("assets/texturas/menus/boton_play.png");
            boton2 = (ImTextureID)getTexture("assets/texturas/menus/boton_options.png");
            boton3 = (ImTextureID)getTexture("assets/texturas/menus/boton_exit2.png");
            boton4 = (ImTextureID)getTexture("assets/texturas/menus/boton_creditos.png");
            boton5 = (ImTextureID)getTexture("assets/texturas/menus/boton_controles.png");
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window,ImGui_ImplGlfw_CursorPosCallback);
            ison=true;
            credits=false;
            break;

        }

        case MENU:
        {

            game.systems.soundito.stopSteps();
            game.systems.soundito.update();
            newState = state(state_id::MENU);
            break;

        }

        case PAUSE:
        {

            game.systems.soundito.stopSteps();
            game.systems.soundito.update();
            newState = state(state_id::PAUSE);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            ison=true;
            boton1 = (ImTextureID)getTexture("assets/texturas/menus/continue.png");
            boton2 = (ImTextureID)getTexture("assets/texturas/menus/boton_options.png");
            boton3 = (ImTextureID)getTexture("assets/texturas/menus/boton_exit2.png");
            boton4 = (ImTextureID)getTexture("assets/texturas/menus/apply.png");
            glfwSetCursorPosCallback(window,ImGui_ImplGlfw_CursorPosCallback);
            credits=false;
           
            break;
        }

        case DEATH:
        {
            game.systems.soundito.stopAll();
            game.systems.soundito.update();
            newState = state(state_id::DEATH);
            break;
        }

        case CODELOCK:
        {
            
            newState = state(state_id::CODELOCK);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window,ImGui_ImplGlfw_CursorPosCallback);
            break;

        }

        case OPTIONS:
        {
            //yera por favor los warnings
            //std::cout << ":)\n";
        }
        case FINAL: 
        {
           
            game.systems.soundito.stopSteps();
            game.systems.soundito.update();
            newState = state(state_id::FINAL);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            ison=true;
            glfwSetCursorPosCallback(window,ImGui_ImplGlfw_CursorPosCallback);
            credits=false;
           
            break;
        }
    }
}

void St_Manager::RemoveState() //se ha pasado codigo de process stat changes aqui
{
    this->states.pop(); //pop en las pilas para el de arriba

        if(!this->states.empty())
        {
            //this->states.top()->resume();
            //this->states.top()->Resume();

        }
}

void St_Manager::ProcessStateChanges()
{
    if(this->isAdding)
    {
        if(!this->states.empty())
        {
            if(this->isReplacing)
            {
                this->states.pop(); //eliminamos y reemplazamos
            }
            else
            {
                //this->states.top()->Pause();
            }

        }
        this->states.push(this->newState);
        this->isAdding = false;
    }
}

void St_Manager::UpdateEngine(GLFWwindow* window, bool iaupdate){ 

    GUI.newFrame();
  
    switch(states.top().type){
        case state_id::PREINTRO:
        {
            frameB.framebuffershader = Shader("src/engine/shaders/VertexShaderFramebuffer.vs","src/engine/shaders/FragmentShaderFramebufferBlack.fs");
            UpdateOpenGL(window, GL, frameB);
            GL.bgintro.Draw(GL.Guishader);
            
            if(fade > 0 && fade < 1)
                game.systems.soundito.playSound("event:/UI/Pop");
            
            game.systems.soundito.update();
            frameB.framebuffershader.use();

            frameB.framebuffershader.setFloat("hp",fade);
            if(fade>1)
            {
                fade += 0.04;
                frameB.framebuffershader.setFloat("hp",1);
            }
            
            else
            {
                fade += 0.005;
            }
           
            renderFramebuffer(frameB);

            afterRenderUpdatesOpenGL(window);
        
            if(fade>10)
            {
                    addState(INTRO,true,window);
                    frameB.framebuffershader = Shader("src/engine/shaders/VertexShaderFramebuffer.vs","src/engine/shaders/FragmentShaderFramebuffer.fs");
            }
            
            break;

        }

        case state_id::INTRO:
        {
            lvls.emptyAll(GE, game.systems, GL);
            //St_Intro st_intro;
            UpdateOpenGL(window, GL, frameB);
            GL.Guishader.use();
            if(options)
            {
                GL.bgoptions.Draw(GL.Guishader);
            }
            if(credits)
            {
                
                if(controls)
                {
                    GL.bgcontrols.Draw(GL.Guishader);
                }
                else
                {
                    GL.bgcredits.Draw(GL.Guishader);
                }
            }
            
            else 
            {
                GL.plano.Draw(GL.Guishader);
            }
            
            frameB.framebuffershader.use();
            frameB.framebuffershader.setFloat("hp",1);
            renderFramebuffer(frameB);

            afterRenderUpdatesOpenGL(window);

            /*if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowSize(window, 1920,1080);
                glfwSetWindowShouldClose(window, true);
            } */
            
            loadmenus(window,false);
            break;

        }
        case state_id::GAME:
        {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE){
                GL.camera.pulsation_release=0;
            }
            
            if(GL.incodelock)
            {
                
                addState(state_id::CODELOCK,false,window);
                           
            }
            GL.camera.ExtractPlanesGL(GL.camera.GetProjectionMatrix()*GL.camera.GetViewMatrix(),true);
            game.game_update(window, GL, frameB, GE, iaupdate);

            if(GL.camera.pulsation_release == 0 && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                GL.camera.pulsation_release=1;
                addState(state_id::PAUSE,false,window);
                    
            }

            if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
            {
                addState(state_id::MENU,false,window);
            }

            if (GL.final_ != 0){ addState(FINAL,true, window); }
            if (GE.getEntityCMP<CHealth>(GE.getEntityArray()[0]).health <= 0)
            {
                addState(state_id::DEATH,true,window);
            }

            /*
            if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
            {
                
                auto& ent = GE.getEntityArray()[2];
                auto& light = GE.getEntityCMP<CPhysics>(ent);
                auto& light2 = GE.getEntityCMP<CInformation>(ent);
                std::cout << light2.description << "\n";
                if(GL.camera.isOnFrustum(glm::vec3(light.x,light.y,light.z)))
                {
                    std::cout << "esta en camara" << "\n";
                }
                else 
                {
                    std::cout << "ya NO esta en camara" << "\n";
                }
            } */

            if (GL.camera.charge_status!=0){
                auto update_lvl = [&](Entity& e, CIDLvl& cidlvl){
                    if(cidlvl.changed != 0){
                        cidlvl.changed = 0;
                        currentselected = 0;
                        GL.camera.charge_status=1;
                        switch (cidlvl.lvlid)
                        {
                        case 1:
                            //std::cout << "lvl 1\n";
                            lvls.clearLevel(GE, game.systems, GL);
                            lvls.load_lvl_1(GE, game.systems, GL);
                            game.systems.rend_sys.initinstances(GE, GL);
                            game.systems.coll_sys.initCollisions(GE);
                            lvls.checkStatus_lvl1(GE, GL);
                            lvls.save_inventory(GE, game.systems, GL);
                            //game.initSystems(); 
                            break;

                        case 2:
                            //std::cout << "lvl 2\n";
                            lvls.clearLevel(GE, game.systems, GL);
                            lvls.load_lvl_2(GE, game.systems, GL);
                            game.systems.rend_sys.initinstances(GE, GL);
                            game.systems.coll_sys.initCollisions(GE);
                            lvls.checkStatus_lvl2(GE,game.systems,GL);
                            lvls.save_inventory(GE, game.systems, GL);
                            //game.initSystems();

                            GL.firstLanternHeld = true;
                            break;
                            //...
                        case 3:
                            lvls.clearLevel(GE, game.systems, GL);
                            lvls.load_lvl_3(GE, game.systems, GL);
                            game.systems.rend_sys.initinstances(GE, GL);
                            game.systems.coll_sys.initCollisions(GE);
                            GL.firstLanternHeld = true;
                            lvls.checkStatus_lvl3(GE,GL);
                            lvls.save_inventory(GE, game.systems, GL);
                            break;
                        case 4:
                            lvls.clearLevel(GE, game.systems, GL);
                            lvls.load_lvl_4(GE, game.systems, GL);
                            game.systems.rend_sys.initinstances(GE, GL);
                            game.systems.coll_sys.initCollisions(GE);
                            GL.firstLanternHeld = true;
                            lvls.checkStatus_lvl4(GE, GL); 
                            lvls.save_inventory(GE, game.systems, GL);
                            break;
                        case 5:
                            //std::cout << "lvl 5\n";
                            lvls.clearLevel(GE, game.systems, GL);
                            lvls.load_lvl_5(GE, game.systems, GL);
                            game.systems.rend_sys.initinstances(GE, GL);
                            game.systems.coll_sys.initCollisions(GE);
                            GL.firstLanternHeld = true;
                            lvls.checkStatus_lvl5(GE, GL);
                            lvls.save_inventory(GE, game.systems, GL);
                            break;
                            //...
                        case 6:
                            //std::cout << "lvl 5\n";
                            lvls.clearLevel(GE, game.systems, GL);
                            lvls.load_lvl_Y(GE, game.systems, GL);
                            game.systems.rend_sys.initinstances(GE, GL);
                            game.systems.coll_sys.initCollisions(GE);
                            lvls.checkStatus_lvlY(GE, GL);
                            lvls.save_inventory(GE, game.systems, GL);
                            break;
                        case 7:
                            //std::cout << "lvl 5\n";
                            lvls.clearLevel(GE, game.systems, GL);
                            lvls.load_lvl_maze(GE, game.systems, GL);
                            game.systems.rend_sys.initinstances(GE, GL);
                            game.systems.coll_sys.initCollisions(GE);
                            lvls.save_inventory(GE, game.systems, GL);
                            lvls.checkStatus_lvl_Maze(GE,GL);
                            break;
                        case 8:
                            //std::cout << "lvl 5\n";
                            lvls.clearLevel(GE, game.systems, GL);
                            lvls.load_lvl_Sotano(GE, game.systems, GL);
                            game.systems.rend_sys.initinstances(GE, GL);
                            game.systems.coll_sys.initCollisions(GE);
                            lvls.checkStatus_lvl_Sotano(GE,GL);
                            lvls.save_inventory(GE, game.systems, GL);
                            break;

                        default:
                            std::cout << "no level found\n";
                            GL.firstLanternHeld = true;
                            
                            break;
                        }
                        //std::cout << unsigned(cidlvl.changed) << "\n";
                    }
                };
                GE.foreach<Typelist<CIDLvl>, Typelist<>>(update_lvl);

                }else{
                    GL.camera.charge_status = 1;
                    game.systems.soundito.stopSteps();
                    game.systems.soundito.update(); 
		            GL.transition.Draw(GL.Guishader);
		            renderFramebuffer(frameB);
                }
            }
        break;
        
        case state_id::MENU:
            {
                UpdateOpenGL(window, GL, frameB);
                GL.Guishader.use();
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE){ 
                    GL.camera.pulsation_release=0;
                }
                if(firsttimemenu)
                {   
                    //std::cout << "hola entro lmao " <<  GE.getInventory().size() <<"\n";
                    for(uint8_t i = 0 ; (uint8_t)i < GE.getInventory().size(); ++i){
                        if(i < 3)
                        {
                            auto& ent = GE.getInventory()[i];
                            if(ent->hasComponent<CPng>()){
                                auto& model = GE.getEntityCMP<CRenderGL>(*ent);
                                model.modelMatrix.setPosition(0,0,0);
                                model.modelMatrix.Scale(8,8,8);
                            }
                        }

                    }
                    firsttimemenu=false;
                }
                
                
                for(uint8_t i = 0 ; (uint8_t)i < GE.getInventory().size(); ++i){
                    if(i < 3)
                    {
                        auto& ent = GE.getInventory()[i];
                        if(ent->hasComponent<CPng>() && ent->hasComponent<CInformation>()){
                            auto& png = GE.getEntityCMP<CPng>(*ent);
                            auto& text = GE.getEntityCMP<CInformation>(*ent);
                            png.icono.setPosition(-0.70,0.55*(i-1.15),0);
                            png.icono.Draw(GL.Guishader);
                            
                            
                            if(i == currentselected)
                            {
                                auto& model = GE.getEntityCMP<CRenderGL>(*ent);
                                model.modelMatrix.setPosition(0,0,-0.5);
                                GL.shaderPlayer.use();
                                model.modelMatrix.Draw(GL.shaderPlayer);

                                if(i==0)
                                {
                                    GL.selector.setPosition(-0.70,0.55*(i-1.05),0);
                                }
                                if(i==1)
                                {
                                    GL.selector.setPosition(-0.70,0.55*(i-1.15),0); //bien
                                }
                                if(i==2)
                                {
                                    GL.selector.setPosition(-0.70,0.55*(i-1.23),0); //tambien
                                }
                                //std::cout << "estoy en i " << unsigned(i) << "\n";
                                
                                GL.selector.Draw(GL.Guishader);
                                GL.renderText(text.description, GL.SRC_WIDTH/2, GL.SRC_HEIGHT/1.3, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
                                
                            }
                        }
                    }
                    
                } 
                GL.inventario.Draw(GL.Guishader);

                frameB.framebuffershader.use();
                frameB.framebuffershader.setFloat("hp",1);
                renderFramebuffer(frameB);
                
                afterRenderUpdatesOpenGL(window);
                
                if(GL.camera.pulsation_release == 0 && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
                    //std::cout << "bajo a " << currentselected << "\n";
                    GL.camera.pulsation_release =1;
                    firsttimemenu=true;
                    for(uint8_t i = 0 ; (uint8_t)i < GE.getInventory().size(); ++i){
                        if(i < 3)
                        {
                            auto& ent = GE.getInventory()[i];
                            if(ent->hasComponent<CPng>()){
                                auto& model = GE.getEntityCMP<CRenderGL>(*ent);
                                model.modelMatrix.setPosition(0,0,0);
                                model.modelMatrix.Scale(0.125,0.125,0.125);
                            }
                        }

                    }
                    RemoveState();
                }
                
                if( GL.camera.pulsation_release == 0 && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
                    GL.camera.pulsation_release = 1;
                    if(currentselected > 0)
                    {
                        currentselected--;
                    }
                }

                if( GL.camera.pulsation_release == 0 && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
                    GL.camera.pulsation_release = 1;
                    if(currentselected < 2 && currentselected < GE.getInventory().size()-1)
                    {
                        currentselected++;
                    }
                }
                if(GE.getInventory().size() != 0 ){

                    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
                        if(GE.getInventory()[currentselected]){
                            auto& ent = GE.getInventory()[currentselected];
                            if(ent->hasComponent<CRenderGL>()){
                                auto& model = GE.getEntityCMP<CRenderGL>(*ent);
                                model.modelMatrix.Rotate(2,1,0,0);
                            }
                        }
                    }
                    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
                        if(GE.getInventory()[currentselected]){
                            auto& ent = GE.getInventory()[currentselected];
                            if(ent->hasComponent<CRenderGL>()){
                                auto& model = GE.getEntityCMP<CRenderGL>(*ent);
                                model.modelMatrix.Rotate(2,0,1,0);
                            }
                        }
                    }
                }
            break;
            }

            case DEATH: 

            UpdateOpenGL(window, GL, frameB);
            GL.Guishader.use();
            GL.death.Draw(GL.Guishader);

            if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                //game.endManNSys();
                lvls.reset_game_variables(GE,game.systems,GL);
                addState(state_id::INTRO,true,window);
                
            }
            
            frameB.framebuffershader.use();
            frameB.framebuffershader.setFloat("hp",1);
            renderFramebuffer(frameB);
            
            afterRenderUpdatesOpenGL(window);
                
            break;

            case PAUSE: 
            {
                
                UpdateOpenGL(window, GL, frameB);
                GL.Guishader.use();
                if(options)
                {
                    GL.bgoptions.Draw(GL.Guishader);
                }

                if(controls)
                {
                    GL.bgcontrols.Draw(GL.Guishader);
                }
                else 
                {
                    
                    GL.bgpause.Draw(GL.Guishader);
                }

                frameB.framebuffershader.use();
                frameB.framebuffershader.setFloat("hp",1);
                renderFramebuffer(frameB);

                afterRenderUpdatesOpenGL(window); 
                
                loadmenus(window,true);
        
                break;
            }
            case FINAL: 
            {
                
                UpdateOpenGL(window, GL, frameB);
                GL.Guishader.use();
                if(GL.final_ == 1)
                {
                    //FINAL MALO
                    GL.bgbadending.Draw(GL.Guishader);
                }
                else 
                {
                    //FINAL BUENISIMMO
                    GL.bggoodending.Draw(GL.Guishader);
                }

                frameB.framebuffershader.use();
                frameB.framebuffershader.setFloat("hp",1);
                renderFramebuffer(frameB);

                afterRenderUpdatesOpenGL(window); 

                ImGui::Begin("Name", NULL,131);
                ImGui::SetNextWindowPos(ImVec2(0,0));
                ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

                posx=GL.SRC_WIDTH/2 - offsetx;
                posy=GL.SRC_HEIGHT/1.2;
                ImGui::SetCursorPos(ImVec2(posx,posy));
                if (ImGui::ImageButton(boton3, ImVec2((float)offsetx*2, (float)offsety*2), ImVec2(-1.0f, 1.0f), ImVec2(0.0f, 0.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) 
                {
                    GL.final_=0;
                    lvls.reset_game_variables(GE,game.systems,GL);
                    addState(state_id::INTRO,true,window);
                }
                ImGui::End();
                break;
            }
            case CODELOCK:
            {
                UpdateOpenGL(window, GL, frameB);
                if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
                    //std::cout << "bajo a " << currentselected << "\n";
                    GL.incodelock=false;
                    initRaton(window);
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    RemoveState();
                }
                game.systems.rend_sys.update(GE, GL);
                //ImGui::SetNextWindowPos(ImVec2(0,0));
                //ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
                //ImGui::Begin("Name", NULL,131);
                frameB.framebuffershader.use();
                frameB.framebuffershader.setFloat("hp",1);
                renderFramebuffer(frameB);

                afterRenderUpdatesOpenGL(window); 

                loadmenus(window, true);
              
                break;
            }
    }
    
    GUI.Render(window);
    
}

unsigned int St_Manager::getTexture(const char *path)
{
    
    std::string filename = std::string(path);
    
    unsigned int textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        
        GLenum format = GL_RED;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
    
}




void St_Manager::loadmenus(GLFWwindow* window,bool isgamerunning)
{

    
    switch(GL.SRC_HEIGHT)
    {
        case 1080: //1080x1920
        {   
            offsetx=75; //la mitad del tamaño del boton, ya que su centro está en la esquina superior
            offsety=30; // si lo multiplicamos x2 podemos manipular el tamaño del boton
            break;
        }
        case 450:
        { 
            offsetx=31;
            offsety=13;
            break;
        }
        case 270:
        { 
            offsetx=19;
            offsety=8;
            break;
        }
        default:
        {
            offsetx=75;
            offsety=30;
        }
    }

    if(credits)
        {
            ImGui::Begin("Name", NULL,131);
            ImGui::SetNextWindowPos(ImVec2(0,0));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
            
            posx=GL.SRC_WIDTH/1.1;
            posy=GL.SRC_HEIGHT/1.1;
            ImGui::SetCursorPos(ImVec2(posx,posy));
        
            if (ImGui::ImageButton(boton3, ImVec2((float)offsetx*2, (float)offsety*2), ImVec2(-1.0f, 1.0f), ImVec2(0.0f, 0.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) 
            {
                credits =false;
                controls=false;
            }
            ImGui::End();
        }

    if(ison && !credits)
    {
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        
        ImGui::Begin("Name", NULL,131);
        posx=GL.SRC_WIDTH/1.2;
        posy=GL.SRC_HEIGHT/1.2;
        ImGui::SetCursorPos(ImVec2(posx,posy));
        if(!isgamerunning)
        {
            if (ImGui::ImageButton(boton4, ImVec2((float)offsetx*2, (float)offsety*2), ImVec2(-1.0f, 1.0f), ImVec2(0.0f, 0.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) 
            {
            credits = true;
            }
        }
        

        posx=GL.SRC_WIDTH/2 - offsetx;
        posy=GL.SRC_HEIGHT/1.4;
        ImGui::SetCursorPos(ImVec2(posx,posy));

        
        if(isgamerunning)
        {
            if (ImGui::ImageButton(boton1, ImVec2((float)offsetx*2, (float)offsety*2), ImVec2(-1.0f, 1.0f), ImVec2(0.0f, 0.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) 
            {           

                RemoveState();
                //std::cout << "intro init XXXXX " <<"\n";
                ison = false;       
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);     
                initRaton(window);
            }
        }
        else
        {
            if (ImGui::ImageButton(boton1, ImVec2((float)offsetx*2, (float)offsety*2), ImVec2(-1.0f, 1.0f), ImVec2(0.0f, 0.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) 
            {
                //std::cout << "intro init " <<"\n";
                //lanzar game             
                
                addState(state_id::GAME,true,window);             
                ison = false;
                initRaton(window);
                
            }
        }
        posy += offsety*2;
        ImGui::SetCursorPos(ImVec2(posx,posy));
        if (ImGui::ImageButton(boton5, ImVec2((float)offsetx*2, (float)offsety*2), ImVec2(-1.0f, 1.0f), ImVec2(0.0f, 0.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) {
            credits=true;
            controls=true;
        }
        posy += offsety*2;
        ImGui::SetCursorPos(ImVec2(posx,posy));
        if (ImGui::ImageButton(boton2, ImVec2((float)offsetx*2, (float)offsety*2), ImVec2(-1.0f, 1.0f), ImVec2(0.0f, 0.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) {
            boton4 = (ImTextureID)getTexture("assets/texturas/menus/apply.png");
            options = !options;
            ison = false;
        }

        posy += offsety*2;
        ImGui::SetCursorPos(ImVec2(posx,posy));
        if(isgamerunning)
        {
            if (ImGui::ImageButton(boton3, ImVec2((float)offsetx*2, (float)offsety*2), ImVec2(-1.0f, 1.0f), ImVec2(0.0f, 0.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) 
            {
            RemoveState();
            RemoveState();

            lvls.reset_game_variables(GE,game.systems,GL);
            addState(state_id::INTRO,true,window);
            isgamerunning=false;
            }
        }
        else
        {
            if (ImGui::ImageButton(boton3, ImVec2((float)offsetx*2, (float)offsety*2), ImVec2(-1.0f, 1.0f), ImVec2(0.0f, 0.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) 
            {
                //game.systems.soundito.drop();
                glfwSetWindowSize(window, 1920,1080);
                glfwSetWindowShouldClose(window, true);
                
            }
        }
        
        
        
        ImGui::End();
    }

    if(options && !credits)
    {
        
        posx=GL.SRC_WIDTH/7;
        posy=GL.SRC_HEIGHT/5;
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Name", NULL,131); 
        ImGui::SetCursorPos(ImVec2(posx,posy));
        //ImGui::Image();
            const char* items[] = { "FULL RES" , "PS2 RES (recommended)" , "PS1 RES","If resolutions dont work, follow download instructions" };
            static const char* current_item = "FULL RES";
          
            ImGuiStyle& style = ImGui::GetStyle();
            
            float w = ImGui::CalcItemWidth();
            float spacing = style.ItemInnerSpacing.x;
            float button_sz = ImGui::GetFrameHeight();
            ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
            if (ImGui::BeginCombo("Resolution", current_item, ImGuiComboFlags_NoArrowButton)){
                for (int n = 0; n < IM_ARRAYSIZE(items); n++){
                    bool is_selected = (current_item == items[n]);
                    if (ImGui::Selectable(items[n], is_selected))
                        current_item = items[n];
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
                    
            posy += offsety*2+25;
            ImGui::SetCursorPos(ImVec2(posx,posy));
            ImGui::Checkbox("Shader TV", &shader);

             if(shader) 
                {
                    frameB.framebuffershader = Shader("src/engine/shaders/VertexShaderFramebuffer.vs","src/engine/shaders/FragmentShaderFramebuffer.fs");
                }
            else 
                {
                    frameB.framebuffershader = Shader("src/engine/shaders/VertexShaderFramebufferNOCRT.vs","src/engine/shaders/FragmentShaderFramebufferNOCRT.fs");
                }

            posy += offsety*2+25;
            ImGui::SetCursorPos(ImVec2(posx,posy));
            ImGui::SliderFloat("##", &soundvalue, 0.0f, 100.0f, "Volume = %.1f");
            //std::cout << "sound value " << soundvalue << "\n";
            game.systems.soundito.setVolume(soundvalue);
            
            posx = GL.SRC_WIDTH/2;
            posy = GL.SRC_WIDTH/2;
            ImGui::SetCursorPos(ImVec2(posx,posy));
            if (ImGui::ImageButton(boton4, ImVec2((float)offsetx*2, (float)offsety*2), ImVec2(-1.0f, 1.0f), ImVec2(0.0f, 0.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f)))  
            {   
                boton4 = (ImTextureID)getTexture("assets/texturas/menus/boton_creditos.png");
                if(strcmp(current_item, "FULL RES")==0)
                {
                    GL.SRC_WIDTH = 1920;
                    GL.SRC_HEIGHT = 1080;
                    //style.ScaleAllSizes(1.0f);
                }

                if(strcmp(current_item, "PS1 RES")==0)
                {
                    GL.SRC_WIDTH = 480;
                    GL.SRC_HEIGHT = 270;

                    //style.ScaleAllSizes(0.5f);
                }
                if(strcmp(current_item, "PS2 RES (recommended)")==0)
                {
                    GL.SRC_WIDTH = 800;
                    GL.SRC_HEIGHT = 450;
                    //style.ScaleAllSizes(0.7f);
                }
                updateGlobals(GL);
                
                frameB.clearbuffer();
                frameB = framebuffer{};
                if(!shader)
                {
                    frameB.framebuffershader = Shader("src/engine/shaders/VertexShaderFramebufferNOCRT.vs","src/engine/shaders/FragmentShaderFramebufferNOCRT.fs");
                } 
                
                glfwSetWindowSize(window, GL.SRC_WIDTH,GL.SRC_HEIGHT);
                options = !options;
                ison = true; 
            }

            posx += offsetx*2;
            ImGui::SetCursorPos(ImVec2(posx,posy));
            if (ImGui::ImageButton(boton3, ImVec2((float)offsetx*2, (float)offsety*2), ImVec2(-1.0f, 1.0f), ImVec2(0.0f, 0.0f), -1, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f)))
            {
                boton4 = (ImTextureID)getTexture("assets/texturas/menus/boton_creditos.png");
                ison = true; 
                options = !options; 
            }

        ImGui::End();
        
    }

    if(GL.incodelock)
    {   
        game.systems.soundito.stopSteps();
        game.systems.soundito.update();
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

        ImGui::Begin("Name", NULL,131); //1986
        
        posx=GL.SRC_WIDTH/2;
        posy=GL.SRC_HEIGHT/3.2;

        
        ImGui::SetCursorPos(ImVec2(posx,posy));
        ImGui::PushItemWidth(100);
        ImGui::InputInt(" ",&codelock1,1,2,0);
        posy +=offsety*1.8+13;
        ImGui::SetCursorPos(ImVec2(posx,posy));
        ImGui::InputInt("  ",&codelock2,1,2,0);
        posy +=offsety*1.8+13;
        ImGui::SetCursorPos(ImVec2(posx,posy));
        ImGui::InputInt("   ",&codelock3,1,2,0);
        posy +=offsety*1.8+13;
        ImGui::SetCursorPos(ImVec2(posx,posy));
        ImGui::InputInt("    ",&codelock4,1,2,0);

        //std::cout << "values " << codelock1 << " value2 " << codelock2 << " value3 " << codelock3 << " value 4 " << codelock4 << "\n";

        posx = GL.SRC_WIDTH/2;
        posy = GL.SRC_WIDTH/2;
        ImGui::SetCursorPos(ImVec2(posx,posy));
        if (ImGui::Button("Exit"))
        {
            GL.incodelock=false;
            initRaton(window);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            RemoveState();
        }

                //if(ImGui::Button("Enter")){
        if(codelock1 == 1 && codelock2 == 9 && codelock3 == 8 && codelock4 == 6)
        {
            ImGui::Text("Puzzle completed");
            GL.incodelock=false;
            initRaton(window);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            RemoveState();
            GL.codelock_completed = true;
            
            
        }

        if (codelock1 > 9) { codelock1 = 0 ;} if (codelock1 < 0) { codelock1 = 9 ;}
        if (codelock2 > 9) { codelock2 = 0 ;} if (codelock2 < 0) { codelock2 = 9 ;}
        if (codelock3 > 9) { codelock3 = 0 ;} if (codelock3 < 0) { codelock3 = 9 ;}
        if (codelock4 > 9) { codelock4 = 0 ;} if (codelock4 < 0) { codelock4 = 9 ;}
        ImGui::End();
    }
}

