#pragma once

#include <glm/glm.hpp>

struct Light
{
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    bool enabled;
    
    Light()
        : position(0.0f)
        , color(1.0f)
        , intensity(1.0f)
        , enabled(true)
    {
    }
    
    Light(const glm::vec3& pos, const glm::vec3& col, float intens, bool en = true)
        : position(pos)
        , color(col)
        , intensity(intens)
        , enabled(en)
    {
    }
};
