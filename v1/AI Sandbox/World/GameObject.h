#pragma once

#include "BehaviorModule.h"

enum class MovementCalculationType
{
    MovementCalculation_AddAcceleration,
    MovementCalculation_SetVelocity
};

//struct BehaviorModulePriorityCompare
//{
//    bool operator() (const BehaviorModule& lhs, const BehaviorModule& rhs)
//    {
//        return lhs.GetPriority() < rhs.GetPriority();
//    }
//
//    bool operator() (const std::shared_ptr<BehaviorModule>& lhs, const std::shared_ptr<BehaviorModule>& rhs)
//    {
//        return lhs->GetPriority() < rhs->GetPriority();
//    }
//};

class World;

class GameObject
{
public:
    GameObject(DirectX::SimpleMath::Vector2 position);
    virtual ~GameObject();

    // Common functions
    void Update(World* world, float elapsedTime);
    void Render(DirectX::SpriteBatch* spriteBatch);
    virtual void RenderDebugInfo(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitiveBatch);

    // Behavior control
    void AddBehaviorModule(std::shared_ptr<BehaviorModule> behaviorModule); // use default priority level for this BehaviorModule
    void AddBehaviorModule(std::shared_ptr<BehaviorModule> behaviorModule, char priority);

    // Texture control
    void CreateTexture(ID3D11Device2* device);
    void ResetTexture();

    // World control
    DirectX::SimpleMath::Vector2 GetAcceleration() { return m_acceleration; }
    float GetFacingAngle() { return m_facingAngle; }
    float GetMaxAcceleration() { return m_maxAcceleration; }
    float GetMaxSpeed() { return m_maxSpeed; }
    DirectX::SimpleMath::Vector2 GetPosition() { return m_position; }
    size_t GetTeamNumber() { return m_teamNumber; }
    DirectX::SimpleMath::Vector2 GetVelocity() { return m_velocity; }

    bool IsValidTarget() { return m_isValidTarget; }

    void AddAcceleration(DirectX::SimpleMath::Vector2 acceleration) { m_accelerationAccumulated += acceleration; }
    void SetAcceleration(DirectX::SimpleMath::Vector2 acceleration) { m_acceleration = acceleration; }
    void SetFacingAngle(float facingAngle) { m_facingAngle = facingAngle; }
    void SetPosition(DirectX::SimpleMath::Vector2 position) { m_position = position; }
    void SetTeamNumber(size_t teamNumber) { m_teamNumber = teamNumber; } // normally should only be used by World methods
    void SetTextureTint(DirectX::SimpleMath::Color tint) { m_textureTint = tint; }
    void SetValidTarget(bool isValidTarget) { m_isValidTarget = isValidTarget; }
    void SetVelocity(DirectX::SimpleMath::Vector2 velocity) { m_velocity = velocity; }

    MovementCalculationType m_movementCalculation;

private:
    DirectX::SimpleMath::Vector2 m_acceleration;
    DirectX::SimpleMath::Vector2 m_accelerationAccumulated;
    std::multimap<char, std::shared_ptr<BehaviorModule>> m_behaviorModules;
    float m_facingAngle;
    bool m_isValidTarget;
    float m_maxAcceleration; // centimeters per second per second
    float m_maxAngularRotation; // radians per second
    float m_maxSpeed; // centimeters per second
    DirectX::SimpleMath::Vector2 m_position;
    float m_speed; // centimeters per second
    size_t m_teamNumber;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
    DirectX::SimpleMath::Vector2 m_textureOrigin;
    DirectX::SimpleMath::Color m_textureTint;
    DirectX::SimpleMath::Vector2 m_velocity;
};
