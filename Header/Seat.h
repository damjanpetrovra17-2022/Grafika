#pragma once

#include <glm/glm.hpp>
#include "Header/AABB.h"

enum class SeatState
{
    Free,
    Reserved,
    Purchased
};

struct Seat
{
    int row;
    int col;
    SeatState state;
    glm::vec3 position;    
    AABB bounds;           
    
    Seat()
        : row(0)
        , col(0)
        , state(SeatState::Free)
        , position(0.0f)
        , bounds()
    {
    }
    
    Seat(int r, int c, const glm::vec3& pos, const AABB& b)
        : row(r)
        , col(c)
        , state(SeatState::Free)
        , position(pos)
        , bounds(b)
    {
    }
};
