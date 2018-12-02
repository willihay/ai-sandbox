#pragma once

#include "BehaviorModule.h"

class GameObject;
class World;

class FollowBehavior : public BehaviorModule
{
public:
    FollowBehavior(std::shared_ptr<GameObject> target = nullptr);
    virtual ~FollowBehavior();

    // Override functions
    virtual void Run(World* world, GameObject* gameObject, float elapsedTime) override;

    // Module-specific functions
    void SetFollowDistance(float distance) { m_followDistance = distance; }

private:
    float m_followDistance;
    std::shared_ptr<GameObject> m_followTarget;
};
