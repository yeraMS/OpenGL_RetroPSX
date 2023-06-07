#include "Animation_system.hpp"
#include "../man/SoundManager.hpp"
#include "../engine/teosinteEngine.hpp"
//struct SoundManager; 

AnimationSystem::AnimationSystem(){}

void AnimationSystem::update(Manentity_type& EM, EngineGL& GL) {

    auto update_one_entity = [&](Entity& e, CAnimation& canim, CRenderGL& crend, CPhysics& phy){
        if(!e.hasTag<TNotRenderAnim>()){
            
            crend.modelMatrix.setPosition(phy.x, phy.y, phy.z);
            canim.animator.UpdateAnimation(GL.deltaTime);

            auto transforms = canim.animator.GetFinalBoneMatrices();
                for (std::size_t i = 0; i < transforms.size(); ++i){

                    GL.animationShader.use();
                    GL.animationShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
                }

            crend.modelMatrix.Draw(GL.animationShader);
        }
    };
    EM.foreach<SYSCMPs, SYSTAGs>(update_one_entity);

};

