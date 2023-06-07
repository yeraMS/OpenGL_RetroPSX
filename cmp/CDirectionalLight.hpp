#pragma once
#include <glm/glm.hpp>

struct CDirectionalLight {
    
    glm::vec3 direction{}, ambient {}, diffuse{}, specular{};
};