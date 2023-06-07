#include "Render_system.hpp"
#include "../engine/teosinteEngine.hpp"

RenderSystem::RenderSystem(){}

void RenderSystem::update(Manentity_type& EM, EngineGL& GL) 
{ 
    auto update_one_entity = [&](Entity& e, CRenderGL& crend, CPhysics& phy)
    { 
        if(!e.hasTag<TNotRenderizable>() )
        {
            if (!e.hasTag<TPlayer>())
            {
                //if(GL.camera.isOnFrustum(glm::vec3(phy.x,phy.y,phy.z)))
                //{

                    crend.modelMatrix.setPosition(phy.x, phy.y, phy.z);
                    crend.modelMatrix.Draw(GL.shader); 

                    //std::cout << "nombre del modelo: " << crend.modelMatrix.modelname << "\n";
                //}
            }
            else
            {
                //std::cout <<  "ID: " << e.getID() << "\n";
                crend.modelMatrix.Draw(GL.shaderPlayer);
            }
        }
        
    };
    EM.foreach<SYSCMPs, SYSTAGs>(update_one_entity);
    
};

 void RenderSystem::updateinstances(Manentity_type& EM, EngineGL& GL)
 {
     auto update_one_entity = [&](Entity& e, CRenderGL& crend, CPhysics& phy)
    {   
        crend.modelMatrix.Draw(GL.instanceshader);
        for(unsigned int i = 0; i < crend.modelMatrix.meshes.size(); i++)
        {
            glBindVertexArray(crend.modelMatrix.meshes[i].VAO);
            glDrawElementsInstanced(
                GL_TRIANGLES, crend.modelMatrix.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, crend.amount
            );
        }
    };
    EM.foreach<SYSCMPs, SYSTAGINSTANCEs>(update_one_entity);
    
 }

 void RenderSystem::initinstances(Manentity_type& EM, EngineGL& GL)
 {
     auto update_one_entity = [&](Entity& e, CRenderGL& crend, CPhysics& phy)
    { 
        crend.amount = GL.ProcessInstanceTXT(crend.instancePath,crend.modelMatrix, GL,crend.iscorn);
    };
    EM.foreach<SYSCMPs, SYSTAGINSTANCEs>(update_one_entity);
    
 }