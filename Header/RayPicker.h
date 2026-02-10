#pragma once

#include "Ray.h"
#include <glm/glm.hpp>

struct AABB;
struct Seat;
class SeatGrid;

class RayPicker
{
public:
    RayPicker();
    ~RayPicker();
    
    
    Ray screenPointToRay(
        double mouseX,
        double mouseY,
        int screenWidth,
        int screenHeight,
        const glm::mat4& view,
        const glm::mat4& projection,
        const glm::vec3& cameraPosition
    ) const;
    
    
    
    bool intersectAABB(const Ray& ray, const AABB& box, float& tNear) const;
    
    
    
    Seat* pickSeat(const Ray& ray, SeatGrid& grid) const;
};
