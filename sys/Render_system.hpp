/*************************************************
**************** RENDER SYSTEM *******************
**************************************************/


#pragma once

#include "../man/utils/types.hpp"

struct EngineGL;
struct RenderSystem {
    using SYSCMPs = Typelist<CRenderGL, CPhysics>; 
	using SYSTAGs = Typelist<>;
    using SYSTAGINSTANCEs = Typelist<TInstance>;
    RenderSystem();
    void initinstances(Manentity_type& EM, EngineGL& GL);
    void update(Manentity_type& EM, EngineGL& GL);
    void updateinstances(Manentity_type& EM, EngineGL& GL);
};

