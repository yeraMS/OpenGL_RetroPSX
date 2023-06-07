/*pragma once

#include "teoNode.hpp"

struct teoNode_Manager{
    using nodeStorage = std::vector<teoNode*>;
/*
    template<typename NodeType, typename... PTs>
    auto& createNodeType(PTs&&... params){
        
        NodeType& node {};

        scene.push_back(std::move(a));
        return node;
    }
    
    void customDeleter(teoNode* a){
        for(auto a: a->getChildren())
            delete a; 
    }


    ~teoNode_Manager(){
        for(teoNode* a : scene){
            if(!a->getChildren().empty())
                customDeleter(a);
            a->~teoNode();
        }
    }


    void createScene(){
        
        teoNode* root = new teoNode(); 
        teoNode* child = new teoNode();          
        teoNode* linterna = new teoNode(); 
        linterna->setName("linternica"); 
        root->addChild(child); 
        root->addChild(linterna); 
        
        child->setName("hijo");
        
        for(auto a : root->getChildren()){

            std::cout << a->getName() << "\n"; 

        }
        scene.push_back(root);
        scene.push_back(child);
        scene.push_back(linterna);

    }

    private:
        nodeStorage scene; 
};*/