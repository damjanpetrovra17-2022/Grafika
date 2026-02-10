#include "../Header/RayPicker.h"
#include "../Header/AABB.h"
#include "../Header/Seat.h"
#include "../Header/SeatGrid.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <limits>

RayPicker::RayPicker()
{
}

RayPicker::~RayPicker()
{
}

Ray RayPicker::screenPointToRay(
    double mouseX,
    double mouseY,
    int screenWidth,
    int screenHeight,
    const glm::mat4& view,
    const glm::mat4& projection,
    const glm::vec3& cameraPosition) const
{
    
    
    float x_ndc = (2.0f * static_cast<float>(mouseX)) / static_cast<float>(screenWidth) - 1.0f;
    float y_ndc = 1.0f - (2.0f * static_cast<float>(mouseY)) / static_cast<float>(screenHeight);
    
    
    glm::vec4 rayClip(x_ndc, y_ndc, -1.0f, 1.0f);
    
    
    glm::mat4 invProj = glm::inverse(projection);
    glm::vec4 rayEye = invProj * rayClip;
    rayEye.z = -1.0f;  
    rayEye.w = 0.0f;   
    
    
    glm::mat4 invView = glm::inverse(view);
    glm::vec4 rayWorld4 = invView * rayEye;
    glm::vec3 rayWorld(rayWorld4.x, rayWorld4.y, rayWorld4.z);
    
    
    rayWorld = glm::normalize(rayWorld);
    
    return Ray(cameraPosition, rayWorld);
}

bool RayPicker::intersectAABB(const Ray& ray, const AABB& box, float& tNear) const
{
    
    
    
    glm::vec3 invDir(
        1.0f / ray.direction.x,
        1.0f / ray.direction.y,
        1.0f / ray.direction.z
    );
    
    
    float t1 = (box.min.x - ray.origin.x) * invDir.x;
    float t2 = (box.max.x - ray.origin.x) * invDir.x;
    float t3 = (box.min.y - ray.origin.y) * invDir.y;
    float t4 = (box.max.y - ray.origin.y) * invDir.y;
    float t5 = (box.min.z - ray.origin.z) * invDir.z;
    float t6 = (box.max.z - ray.origin.z) * invDir.z;
    
    
    float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));
    
    
    if (tmax < 0.0f)
    {
        return false;
    }
    
    
    if (tmin > tmax)
    {
        return false;
    }
    
    
    
    
    tNear = (tmin < 0.0f) ? tmax : tmin;
    
    return true;
}

Seat* RayPicker::pickSeat(const Ray& ray, SeatGrid& grid) const
{
    float closestDistance = std::numeric_limits<float>::max();
    Seat* closestSeat = nullptr;
    
    
    for (int row = 0; row < SeatGrid::ROWS; ++row)
    {
        for (int col = 0; col < SeatGrid::COLS; ++col)
        {
            Seat* seat = grid.getSeat(row, col);
            if (!seat)
                continue;
            
            float tNear;
            if (intersectAABB(ray, seat->bounds, tNear))
            {
                
                if (tNear > 0.0f && tNear < closestDistance)
                {
                    closestDistance = tNear;
                    closestSeat = seat;
                }
            }
        }
    }
    
    return closestSeat;
}
