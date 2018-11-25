#include "pch.h"
#include "GameObject.h"
#include "PlayerInput.h"
#include "World.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

PlayerInput::PlayerInput(InputResources* inputResources) :
    m_inputResources(inputResources),
    m_moveTarget(Vector2::Zero),
    m_useMoveTarget(false)
{
}

PlayerInput::~PlayerInput()
{
}

void PlayerInput::Run(World* world, GameObject* object, float elapsedTime)
{
    // Handle player input for the game object.
    if (m_inputResources)
    {
        auto kbTracker = m_inputResources->GetKeyboardTracker();
        auto mouseTracker = m_inputResources->GetMouseTracker();
        using ButtonState = Mouse::ButtonStateTracker::ButtonState;

        if (mouseTracker.leftButton == ButtonState::PRESSED)
        {
            auto mouseState = mouseTracker.GetLastState();
            m_moveTarget = Vector2(mouseState.x, mouseState.y);
            m_useMoveTarget = true;
        }
    }

    if (m_useMoveTarget)
    {
        auto vectorToTarget = m_moveTarget - object->GetPosition();
        auto distanceToTarget = vectorToTarget.Length();
        auto maxSpeed = object->GetMaxSpeed();

        object->m_movementCalculation = distanceToTarget < maxSpeed ?
            MovementCalculationType::MovementCalculation_SetVelocity :
            MovementCalculationType::MovementCalculation_AddAcceleration;

        if (object->m_movementCalculation == MovementCalculationType::MovementCalculation_SetVelocity)
        {
            auto newSpeed = std::min(maxSpeed, distanceToTarget);
            if (newSpeed < 1.f)
            {
                newSpeed = 0.f;
                object->SetVelocity(Vector2::Zero);
                m_useMoveTarget = false;
            }
            else
            {
                vectorToTarget *= (newSpeed / distanceToTarget);

                object->SetFacingAngle(atan2f(vectorToTarget.y, vectorToTarget.x));
                object->SetVelocity(vectorToTarget);
            }
        }
        else if (object->m_movementCalculation == MovementCalculationType::MovementCalculation_AddAcceleration)
        {
            auto maxAcceleration = object->GetMaxAcceleration();
            auto acceleration = vectorToTarget * (maxAcceleration / distanceToTarget);
            object->AddAcceleration(acceleration);
        }
    }
}

void PlayerInput::RenderDebugInfo(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitiveBatch)
{
    if (m_useMoveTarget)
    {
        // Draw triangle at target position.
        Vector2 targetPosition = m_moveTarget;
        VertexPositionColor v1(Vector3(targetPosition.x, targetPosition.y - 2.f, 0.f), Colors::White);
        VertexPositionColor v2(Vector3(targetPosition.x + 2.f, targetPosition.y + 2.f, 0.f), Colors::White);
        VertexPositionColor v3(Vector3(targetPosition.x - 2.f, targetPosition.y + 2.f, 0.f), Colors::White);
        primitiveBatch->DrawTriangle(v1, v2, v3);
    }
}
