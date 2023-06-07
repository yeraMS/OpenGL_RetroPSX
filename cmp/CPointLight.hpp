#pragma once
#include <cstdint>

struct CPointLight {
  
  //  glm::vec3 ambient {}, diffuse{}, specular{};
  float ambientx{}, ambienty{}, ambientz{};
  float diffusex{}, diffusey{}, diffusez{};
  float specularx{}, speculary{}, specularz{};
  float constant{}, linear{}, quadratic{};
  
  std::size_t id {};
  
  void setOff();
  void setOnLamp();
  void debugLight();
  void bigLight();
  void smallLight();
  void houseLight();
  bool isOn;
  

  
    
};