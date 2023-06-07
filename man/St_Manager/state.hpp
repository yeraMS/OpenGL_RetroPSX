#pragma once
class state
{
    public:
    int type;
    state();
    ~state()=default;
    state(int type);

    void run();

    void resume();

};