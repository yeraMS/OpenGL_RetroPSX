#include "state.hpp"
#include <iostream>


state::state(){}
state::state(int type)
{
    this->type = type;
}


void state::run()
{
    std::cout << "state base " << "\n";
}

void state::resume() {}