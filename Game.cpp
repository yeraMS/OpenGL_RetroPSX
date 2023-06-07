#include "Game.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>

//INCLUDE ENGINE OPENGL

// INCLUDE MANAGERS
#include "engine/teosinteEngine.hpp"
#include "engine/animation.h"
#include "engine/animator.h"
//INCLUDE LEVELS
//TYPES 
#include "man/utils/types.hpp"
#include "man/utils/Timer.hpp"



// GAME STRUCT



Game::Game(){};
 // SYS STRUCT 
Sys::Sys(){
    //s_mgr = new SoundManager();
}



void Game::game_update(GLFWwindow* window, EngineGL& GL, framebuffer frameB, Manentity_type& man_e, bool iaupdate){
	
	UpdateOpenGL(window, GL, frameB);
	if (GL.camera.charge_plane == 0){
	systems.input_sys.update(window, man_e, GL);
	
	if(iaupdate){
		systems.ia_sys.update(man_e, GL);
	}
	systems.phy_sys.update(man_e);
	systems.health_sys.update(man_e);
	systems.PL_sys.update(man_e,GL);
	systems.coll_sys.update(man_e, GL, window);
	systems.rend_sys.update(man_e, GL);
	systems.Ani_sys.update(man_e, GL);
	UpdateShaderPlayer(GL);
	renderLights(window,GL);

	
	
	//std::cout << GL.camera.Position.x << "  "<< GL.camera.Position.y<< "  " << GL.camera.Position.z << "\n";
	update_candil(GL,man_e);

	//GL.candilResource.setPosition(-0.5,-0.85, 0);
	renderGui(GL);
	renderskybox(GL); //RENDER SKYBOX Y AFTER RENDER UPDATES SIEMPRE LO ULTIMO Y EN ESE ORDEN
	renderinstances(GL, man_e); // IMPORTANTE    el man_e es porque no es global el candil, necesitamos su luz
	systems.rend_sys.updateinstances(man_e, GL);


	frameB.framebuffershader.use();
    frameB.framebuffershader.setFloat("hp",man_e.getEntityCMP<CHealth>(man_e.getEntityArray()[0]).health/100);
	
    renderFramebuffer(frameB);
    

	//auto& ent = man_e.getEntityArray()[0];
    //auto& light = man_e.getEntityCMP<CHealth>(ent);
	//light.health += 0.1;


	//afterRenderUpdatesOpenGL(window);
	systems.sound_sys.update(man_e, GL); 
	systems.input_sys.resetPlayerVelocity(man_e);
	}else{
		//renderGui(GL); for debug
		if (GL.camera.charge_plane != 0){}
		systems.soundito.stopSteps();
		GL.transition.Draw(GL.Guishader);
		renderFramebuffer(frameB);
		GL.camera.charge_plane--;
	}
}

void Game::update_candil( EngineGL& GL, Manentity_type& man_e)
{
	auto& ent = man_e.getEntityArray()[2];
    auto& light = man_e.getEntityCMP<CPointLight>(ent);
	float pos = -0.8f;
	float scale = 1;
	float currenthealth = 0;

	if(light.isOn) 
	{
		if(man_e.getEntityCMP<CHealth>(man_e.getEntityArray()[2]).health > 14)
		{
			man_e.getEntityCMP<CHealth>(man_e.getEntityArray()[2]).health -= 0.05;
			currenthealth = man_e.getEntityCMP<CHealth>(man_e.getEntityArray()[2]).health / 100; 
		}
		else
		{
			light.setOff();
			man_e.getEntityCMP<CHealth>(man_e.getEntityArray()[2]).health = 0;
			man_e.getEntityCMP<CSound>(man_e.getEntityArray()[2]).s_mgr->setParameter("event:/Character/Fire (Oil)", "Progreso", 10 );
		}
	}
	else
	{
			currenthealth = man_e.getEntityCMP<CHealth>(man_e.getEntityArray()[2]).health / 100; 
	}
	
	GL.candilResource.nodeMatrix = glm::mat4(1.0f);
	GL.candilResource.Rotate(90,0,1,0);
    GL.candilResource.Rotate(-90,1,0,0);
	GL.candilResource.Scale(0.2,0.2,-0.3);
	if(man_e.getEntityCMP<CHealth>(man_e.getEntityArray()[2]).health < 50 && man_e.getEntityCMP<CHealth>(man_e.getEntityArray()[2]).health > 2)
	{
		pos -= (1/(man_e.getEntityCMP<CHealth>(man_e.getEntityArray()[2]).health/100))/100;
		scale -= ((1/(man_e.getEntityCMP<CHealth>(man_e.getEntityArray()[2]).health/100))/10) * 2;
	}
	
	GL.candilResource.setPosition(-0.8f, pos, 0.0f);
	
	GL.candilResource.Scale(scale,1,currenthealth);
}
