#pragma once

namespace Helper
{
// Initialize random number system
void RandomInit();

// Generate random angle (in radians) in the range [0,2Pi]
inline float RandomAngle()
{
    float angle = float(rand()) / float(RAND_MAX);
    return angle * DirectX::XM_2PI;
}

// Generate random integer in the range [min,max] (if max-min <= RAND_MAX(32767))
inline int32_t RandomBetween(int32_t min, int32_t max)
{
    return (rand() % (max - min + 1)) + min;
}

// Generate random float in the range [min,max] (if max-min <= RAND_MAX(32767))
inline float RandomBetween(float min, float max)
{
    float f = float(rand()) / float(RAND_MAX);
    return min + (f * (max - min));
}

// Generate random screen position in the viewport range [TopLeft,BottomRight) (assuming width and height are both <= RAND_MAX(32767))
inline DirectX::SimpleMath::Vector2 RandomScreenPosition(D3D11_VIEWPORT viewport)
{
    return DirectX::SimpleMath::Vector2(
        float(RandomBetween(int32_t(viewport.TopLeftX), int32_t(viewport.TopLeftX + viewport.Width - 1))),
        float(RandomBetween(int32_t(viewport.TopLeftY), int32_t(viewport.TopLeftY + viewport.Height - 1))));
}

// Generate random unit Vector2
inline DirectX::SimpleMath::Vector2 RandomDirection2D()
{
    float angle = RandomAngle();
    return DirectX::SimpleMath::Vector2(std::cosf(angle), std::sinf(angle));
}
}