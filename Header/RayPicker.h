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
    
    // Convert screen coordinates to world-space ray
    Ray screenPointToRay(
        double mouseX,
        double mouseY,
        int screenWidth,
        int screenHeight,
        const glm::mat4& view,
        const glm::mat4& projection,
        const glm::vec3& cameraPosition
    ) const;
    
    // Test ray intersection with AABB
    // Returns true if hit, sets tNear to intersection distance
    bool intersectAABB(const Ray& ray, const AABB& box, float& tNear) const;
    
    // Pick the closest seat hit by the ray
    // Returns nullptr if no seat was hit
    Seat* pickSeat(const Ray& ray, SeatGrid& grid) const;
};
