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

    virtual void Run(World* world, GameObject* object, float elapsedTime) override;
    virtual void RenderDebugInfo(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitiveBatch) override;

    virtual char GetDefaultPriorityLevel() const override { return Config::PlayerInput_DefaultPriorityLevel; }

private:
    InputResources* m_inputResources;

    DirectX::SimpleMath::Vector2 m_moveTarget;
    bool m_useMoveTarget;
};
