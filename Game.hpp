#pragma once


#include "sys/Information_system.hpp"
#include "sys/Render_system.hpp"
#include "sys/Physics_system.hpp"
#include "sys/Input_system.hpp"
#include "sys/IA_system.hpp"

#include "sys/PointLight_system.hpp"

#include "sys/Animation_system.hpp"
#include "sys/Collision_system.hpp"
#include "sys/Health_system.hpp"
#include "sys/Sound_system.hpp"
#include "man/SoundManager.hpp"
#include "sys/dt/dt.hpp"

struct framebuffer;


struct Sys {
	Sys();
	RenderSystem rend_sys;
	PhysicsSystem phy_sys;
	InformationSystem inf_sys;
	InputSystem input_sys;
	IASystem ia_sys;

	PointLight_System PL_sys;
	AnimationSystem Ani_sys;
	
	Collision_System coll_sys;
	Sound_system sound_sys;
	HealthSystem health_sys;
	DecisionTree dtree;
	SoundManager soundito;
};
struct Game {

	Game();
	//void initEntities(Manentity_type& man_e);
		
	void game_update(GLFWwindow* window, EngineGL& GL, framebuffer frameB, Manentity_type& man_e, bool iaupdate);
	void update_candil(EngineGL& GL, Manentity_type& man_e);
	Sys systems;
};
