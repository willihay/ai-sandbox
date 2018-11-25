#include "pch.h"
#include "FollowBehavior.h"
#include "GameObject.h"
#include "World.h"

using namespace Config;
using namespace DirectX;
using namespace DirectX::SimpleMath;

FollowBehavior::FollowBehavior(std::shared_ptr<GameObject> target) :
    m_followDistance(Follow_DefaultDistance),
    m_followTarget(target)
{
}

FollowBehavior::~FollowBehavior()
{
}

void FollowBehavior::Run(World* world, GameObject* object, float elapsedTime)
{
    // See if follow target has become invalid.
    if (m_followTarget && !m_followTarget->IsValidTarget())
    {
        m_followTarget = nullptr;
        object->SetVelocity(object->GetVelocity() * 0.5f); // reduce speed by half
        return; // we'll try to acquire a new target next time
    }

    // If there's no follow target, see if a new one can be acquired.
    if (!m_followTarget)
    {
        auto player = world->GetPlayer(0, 0);
        if (player)
        {
            m_followTarget = player;
        }
        else
        {
            return;
        }
    }

    auto vectorToPlayer = m_followTarget->GetPosition() - object->GetPosition();
    auto newSpeed = std::min(object->GetMaxSpeed(), vectorToPlayer.Length() - m_followDistance);

    vectorToPlayer.Normalize();
    vectorToPlayer *= newSpeed;
    object->SetVelocity(vectorToPlayer);
}
