#include "Health_system.hpp"
#include "../man/SoundManager.hpp"

//struct SoundManager; 

HealthSystem::HealthSystem(){}

void HealthSystem::update(Manentity_type& EM) {

    auto update_one_entity = [&](Entity& e, CHealth& chealth){
        
        if (e.hasTag<TNotRenderizable>() && chealth.health<100){
            chealth.health += 0.2;
        }

        if (e.hasTag<TPlayer>()){
            //if(chealth.health < 0){
                //EM.getEntityCMP<CSound>(e).s_mgr->playSound("event:/Character/LowHp");
            //}
        }
    };
    EM.foreach<SYSCMPs, SYSTAGs>(update_one_entity);

};

