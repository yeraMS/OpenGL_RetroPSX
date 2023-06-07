#pragma once

struct Point{
    float x{}, y{}, z{};
};

struct CCollision   { 
    Point minInit;
    Point maxInit;
    Point min;
    Point max;
};