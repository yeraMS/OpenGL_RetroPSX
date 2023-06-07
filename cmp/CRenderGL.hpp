#pragma once
#include "../engine/model.h"


struct CRenderGL {
    Model modelMatrix;
    int amount{0};
    const char * instancePath{""};
    bool iscorn{false};
};