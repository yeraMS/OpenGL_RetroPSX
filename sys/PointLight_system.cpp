#include "PointLight_system.hpp"

#include "../engine/teosinteEngine.hpp"





void PointLight_System::update(Manentity_type& EM,EngineGL& GL){
	auto update_one_entity = [&](Entity& e, CPhysics& cPhy, CPointLight& CPointLight){
	GL.shader.use();
    GL.shader.setInt("maxLights",GL.nextIDLight);
    //std::cout << "max luces " << GL.nextIDLight << "\n";
    //std::cout << "Luz actual "  << CPointLight.id << "\n";
	GL.shader.setVec3("pointLights[" + std::to_string(CPointLight.id) + "].position", cPhy.x,cPhy.y,cPhy.z);
    GL.shader.setVec3("pointLights[" + std::to_string(CPointLight.id) + "].ambient", CPointLight.ambientx, CPointLight.ambienty, CPointLight.ambientz);
    GL.shader.setVec3("pointLights[" + std::to_string(CPointLight.id) + "].diffuse", CPointLight.diffusex, CPointLight.diffusey, CPointLight.diffusez);
    GL.shader.setVec3("pointLights[" + std::to_string(CPointLight.id) + "].specular", CPointLight.specularx, CPointLight.speculary, CPointLight.specularz);
    GL.shader.setFloat("pointLights[" + std::to_string(CPointLight.id) + "].constant", CPointLight.constant);
    GL.shader.setFloat("pointLights[" + std::to_string(CPointLight.id) + "].linear", CPointLight.linear);
    GL.shader.setFloat("pointLights[" + std::to_string(CPointLight.id) + "].quadratic", CPointLight.quadratic);

    GL.shaderPlayer.use();
    GL.shaderPlayer.setInt("maxLights",GL.nextIDLight);
    
    GL.shaderPlayer.setVec3("pointLights[" + std::to_string(CPointLight.id) + "].position", cPhy.x-GL.camera.Position.x, cPhy.y-GL.camera.Position.y, cPhy.z-GL.camera.Position.z);
    if(CPointLight.id == 0)
    {
        GL.shaderPlayer.setVec3("pointLights[" + std::to_string(CPointLight.id) + "].position", 0, 0, 0);
    }
    GL.shaderPlayer.setVec3("pointLights[" + std::to_string(CPointLight.id) + "].ambient", CPointLight.ambientx/1.5, CPointLight.ambienty/1.5, CPointLight.ambientz/1.5);
    GL.shaderPlayer.setVec3("pointLights[" + std::to_string(CPointLight.id) + "].diffuse", CPointLight.diffusex, CPointLight.diffusey, CPointLight.diffusez);
    GL.shaderPlayer.setVec3("pointLights[" + std::to_string(CPointLight.id) + "].specular", CPointLight.specularx, CPointLight.speculary, CPointLight.specularz);
    GL.shaderPlayer.setFloat("pointLights[" + std::to_string(CPointLight.id) + "].constant", CPointLight.constant);
    GL.shaderPlayer.setFloat("pointLights[" + std::to_string(CPointLight.id) + "].linear", CPointLight.linear);
    GL.shaderPlayer.setFloat("pointLights[" + std::to_string(CPointLight.id) + "].quadratic", CPointLight.quadratic);

    };
	EM.foreach<SYSCMPs, SYSTAGs>(update_one_entity);
};