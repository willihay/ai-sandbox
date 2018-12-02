#pragma once

#include "BehaviorModule.h"
#include "InputResources.h"

enum class PlayerInputType
{
    PlayerInput_Keyboard,
    PlayerInput_Mouse
};

class GameObject;
class World;

class PlayerInput : public BehaviorModule
{
public:
    PlayerInput(InputResources* inputResources);
    virtual ~PlayerInput();

    // Override functions
    virtual char GetDefaultPriorityLevel() const override { return Config::PlayerInput_DefaultPriorityLevel; }
    virtual void RenderDebugInfo(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitiveBatch) override;
    virtual void Run(World* world, GameObject* object, float elapsedTime) override;

private:
    InputResources* m_inputResources;

    DirectX::SimpleMath::Vector2 m_moveTarget;
    bool m_useMoveTarget;
};
