#pragma once
#include "../man/utils/types.hpp"
struct EngineGL;
struct AnimationSystem {
	using SYSCMPs = Typelist<CAnimation,CRenderGL,CPhysics>;
	using SYSTAGs = Typelist<>;
	
    AnimationSystem();
    void update(Manentity_type& EM,EngineGL& GL);
};