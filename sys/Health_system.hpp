#pragma once
#include "../man/utils/types.hpp"

struct HealthSystem {
	using SYSCMPs = Typelist<CHealth>;
	using SYSTAGs = Typelist<>;
	
    HealthSystem();
    void update(Manentity_type& EM);
};