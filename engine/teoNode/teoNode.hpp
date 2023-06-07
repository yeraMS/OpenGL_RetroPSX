/*#pragma once
#include <vector>
#include <string>
#include <memory>

#include "../model.h"
//#include "entity.hpp"

struct teoNode{
    using storage = std::vector<teoNode*>;

    teoNode(glm::mat4 m,
            /*Entity *e,*/
            teoNode *n, 
            glm::vec3 trans,
            glm::vec3 rot, 
            glm::vec3 sca, 
            std::string nm, 
            Model mod, 
            glm::vec3 p) : matrixTransf{m}, /*entity{e},*/ traslation{trans}, rotation{rot}, scale{sca}, name{nm}, model{mod}, pos{p}
            {

            }

    teoNode() {
        matrixTransf = glm::mat4(1.0f);
        /*entity       = new Entity(); */
        traslation   = glm::vec3(0.0f,0.0f,0.0f);
        rotation     = glm::vec3(0.0f,0.0f,0.0f);
        scale        = glm::vec3(1.0f,1.0f,1.0f); 
        
        name         = "NoName";
        model        = Model();
        pos          = glm::vec3(1.0f,1.0f,1.0f);
    }

	
//TODO
    void runScene(){
        //En esta función tenemos que dibujar la escena y llamar a los hijos de cada nodo. En principio esta función solo se llama si eres el nodo padre
        //que llama en cascada a los hijos pero todos tienen que llamar a esta función para dibujarse

        if(!children.empty()){

        }
    }


    auto& addChild(teoNode* newChild){
        children.push_back(newChild);
        return children.back(); 
    }

    virtual bool IsComposite() const {
        return false;
    }

    virtual bool IsCamera() const {
        return false;
    }

    virtual bool IsLight() const {
        return false;
    }

    glm::mat4 getmatrixTransf(){
        return matrixTransf; 
    }  

    //Entity *getEntity(){
    //    return entity; 
    //}

    glm::vec3 getTraslation(){
        return traslation; 
    }

    glm::vec3 getRotation(){
        return rotation; 
    }

    glm::vec3 getScale(){
        return scale; 
    }

    storage getChildren(){
        return children;
    }

    std::string getName(){
        return name; 
    }

    Model getModel(){
        return model; 
    }

    glm::vec3 getPos(){
        return pos; 
    }

    teoNode *getParent(){
        return parent; 
    }

    void setmatrixTransf(glm::mat4 matrixTransf){
        this->matrixTransf = matrixTransf;

    }

    //void setEntity(Entity* entity){
    //    this->entity = entity; 
    //} 
 
    void setTranslation(glm::vec3 translation){
        this->traslation = translation; 
    }

    void setRotation(glm::vec3 rotation){
        this->rotation = rotation; 
    }

    void setScale(glm::vec3 scale){
        this->scale = scale;
    }

    void setParent(teoNode* parent){
        this->parent = parent;
    }

    void setName(std::string newname){
        this->name = newname; 
    }

 
    protected: 

        glm::mat4   matrixTransf;
        //Entity *entity;
        glm::vec3   traslation;
        glm::vec3   rotation;
        glm::vec3   scale;
        storage     children;
        teoNode*    parent;
        std::string name;
        Model       model;
        glm::vec3   pos;
};
*/