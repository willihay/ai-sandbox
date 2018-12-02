#pragma once

#include "BehaviorModule.h"

enum class MovementCalculationType
{
    MovementCalculation_AddForces,
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
    GameObject(DirectX::SimpleMath::Vector2 position, ID3D11Device2* device);
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
    void AddForce(DirectX::SimpleMath::Vector2 force) { m_forceAccumulated += force; }
    void AddForceAtPosition(DirectX::SimpleMath::Vector2 force, DirectX::SimpleMath::Vector2 position);
    void AddImpulseAtPosition(DirectX::SimpleMath::Vector2 impulse, DirectX::SimpleMath::Vector2 position);
    void AddTorque(float torque) { m_torqueAccumulated += torque; }

    DirectX::SimpleMath::Vector2 GetAcceleration() { return m_acceleration; }
    float GetRotation() { return m_rotation; }
    float GetMaxAcceleration() { return m_maxAcceleration; }
    float GetMaxSpeed() { return m_maxSpeed; }
    DirectX::SimpleMath::Vector2 GetPosition() { return m_position; } 
    float GetSpeed() { return m_speed; }
    size_t GetTeamNumber() { return m_teamNumber; }
    DirectX::SimpleMath::Vector2 GetVelocity() { return m_velocity; }

    bool IsValidTarget() { return m_isValidTarget; }

    void SetRotation(float rotation) { m_rotation = rotation; }
    void SetPosition(DirectX::SimpleMath::Vector2 position) { m_position = position; }
    void SetTeamNumber(size_t teamNumber) { m_teamNumber = teamNumber; } // normally should only be used by World methods
    void SetTextureTint(DirectX::SimpleMath::Color tint) { m_textureTint = tint; }
    void SetValidTarget(bool isValidTarget) { m_isValidTarget = isValidTarget; }
    void SetVelocity(DirectX::SimpleMath::Vector2 velocity) { m_velocity = velocity; }

    MovementCalculationType m_movementCalculation;

private:
    // Position
    DirectX::SimpleMath::Vector2 m_position;
    float m_rotation; // radians

    // Velocity
    float m_angularVelocity; // radians per second
    float m_maxAngularVelocity; // radians per second
    float m_maxSpeed; // meters per second
    float m_speed; // meters per second
    DirectX::SimpleMath::Vector2 m_velocity; // meters per second

    // Acceleration
    DirectX::SimpleMath::Vector2 m_acceleration; // meters per second per second
    float m_maxAcceleration; // meters per second per second

    // Forces
    DirectX::SimpleMath::Vector2 m_forceAccumulated; // Newtons
    float m_torqueAccumulated;

    // Mass
    float m_mass; // kilograms
    float m_inertia;

    // Shape, Texture, and Other Material Characteristics
    float m_coefficientFriction;
    float m_coefficientRestitution;
    // Shape m_shape; // TODO: includes functions for calculating inertia, getting collision bounds
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
    DirectX::SimpleMath::Vector2 m_textureOrigin;
    DirectX::SimpleMath::Color m_textureTint;

    // Behavior
    std::multimap<char, std::shared_ptr<BehaviorModule>> m_behaviorModules;

    // Other
    bool m_isValidTarget;
    size_t m_teamNumber;
};
