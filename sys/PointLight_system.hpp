/*************************************************
************* INFORMATION SYSTEM *****************
**************************************************/
#pragma once
#include "../man/utils/types.hpp"


struct EngineGL;

struct PointLight_System {
	using SYSCMPs = Typelist<CPhysics, CPointLight>;
	using SYSTAGs = Typelist<>;

	void update(Manentity_type& EM,EngineGL& GL);
};