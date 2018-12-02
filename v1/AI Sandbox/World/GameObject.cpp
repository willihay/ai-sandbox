#include "pch.h"
#include "GameObject.h"
#include "WICTextureLoader.h"
#include "World.h"

using namespace Config;
using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

GameObject::GameObject(Vector2 position, ID3D11Device2* device) :
    m_acceleration(Vector2::Zero),
    m_angularVelocity(0.f),
    m_coefficientFriction(GameObject_DefaultCoefficientFriction),
    m_coefficientRestitution(GameObject_DefaultCoefficientRestitution),
    m_forceAccumulated(Vector2::Zero),
    m_inertia(0.f),
    m_isValidTarget(true),
    m_mass(GameObject_DefaultMass),
    m_maxAcceleration(GameObject_DefaultMaxAcceleration),
    m_maxAngularVelocity(GameObject_DefaultMaxAngularVelocity),
    m_maxSpeed(GameObject_DefaultMaxSpeed),
    m_movementCalculation(MovementCalculationType::MovementCalculation_AddForces),
    m_position(position),
    m_rotation(0.f),
    m_speed(0.f),
    m_teamNumber(0),
    m_textureOrigin(Vector2::Zero),
    m_textureTint(Colors::White.v),
    m_torqueAccumulated(0.f),
    m_velocity(Vector2::Zero)
{
    if (device)
    {
        CreateTexture(device);
    }
}

GameObject::~GameObject()
{
}

void GameObject::Update(World* world, float elapsedTime)
{
    // Run behavior modules.
    for (const auto& behaviorModuleMapPair : m_behaviorModules)
    {
        auto behaviorModule = behaviorModuleMapPair.second;
        if (behaviorModule->IsEnabled())
        {
            behaviorModule->Run(world, this, elapsedTime);
        }
    }
    
    // Apply friction.
    auto friction = -m_velocity;
    friction.Normalize();
    friction *= world->GetFrictionCoefficient() * m_mass * World_Gravity;
    m_forceAccumulated += friction;

    // TODO: verify this "rotational friction" is valid
    m_angularVelocity -= m_angularVelocity * world->GetFrictionCoefficient();

    auto right = Vector2(-std::sinf(m_rotation), std::cosf(m_rotation));
    auto forward = Vector2(std::cosf(m_rotation), std::sinf(m_rotation));

    // ** Update position and velocity using Semi-implicit Euler Method integration (https://en.wikipedia.org/wiki/Semi-implicit_Euler_method)

    // Calculate acceleration.
    m_acceleration = m_forceAccumulated / m_mass;
    float angularAcceleration = m_torqueAccumulated / m_inertia;

    // Update velocity.
    m_velocity += m_acceleration * elapsedTime;
    m_angularVelocity += angularAcceleration * elapsedTime;

    // Apply drag.
    //auto linearDrag = -m_velocity * (world->GetFrictionCoefficient() * elapsedTime);
    //m_velocity += linearDrag;

    // Update speed.
    m_speed = m_velocity.Length();

    // Adjust speed and velocity as necessary for min and max threshholds.
    if (m_speed < 0.1f)
    {
        m_speed = 0.f;
        m_velocity = Vector2::Zero;
    }
    else if (m_speed > m_maxSpeed)
    {
        m_velocity *= m_maxSpeed / m_speed;
    }

    if (std::abs(m_angularVelocity) < 0.001f)
    {
        m_angularVelocity = 0.f;
    }
    else if (m_angularVelocity > m_maxAngularVelocity)
    {
        m_angularVelocity *= m_maxAngularVelocity / m_angularVelocity;
    }

    // Update position.
    m_position += m_velocity * elapsedTime;
    m_rotation += m_angularVelocity * elapsedTime;
    // TODO: use torque and angular velocity for object rotation instead of replacing the above
    //       calculation with a rotation in the direction of velocity

    // Turn the object towards its velocity. (TODO: TO BE REPLACED)
    if (m_speed > 0.f)
    {
        m_rotation = std::atan2f(m_velocity.y, m_velocity.x);
    }
    // TODO: limit turn amount based on max angular rotation speed.

    // TODO: move this boundary check into World Update() under collision detection / resolution
    // Check boundaries and reflect off walls if necessary.
    auto worldRect = world->GetWorldBoundary();
    if (m_position.x > worldRect.x)
    {
        m_position.x = worldRect.x;
        m_velocity = Vector2::Reflect(m_velocity, Vector2(-1, 0));
    }
    else if (m_position.x < 0)
    {
        m_position.x = 0.f;
        m_velocity = Vector2::Reflect(m_velocity, Vector2(1, 0));
    }
    else if (m_position.y > worldRect.y)
    {
        m_position.y = worldRect.y;
        m_velocity = Vector2::Reflect(m_velocity, Vector2(0, -1));
    }
    else if (m_position.y < 0)
    {
        m_position.y = 0.f;
        m_velocity = Vector2::Reflect(m_velocity, Vector2(0, 1));
    }

    // Reset accumulated forces in preparation for the next frame.
    m_forceAccumulated = Vector2::Zero;
    m_torqueAccumulated = 0.f;
}

void GameObject::Render(SpriteBatch* spriteBatch)
{
    spriteBatch->Draw(m_texture.Get(), m_position, nullptr, m_textureTint, m_rotation, m_textureOrigin);
}

void GameObject::RenderDebugInfo(PrimitiveBatch<VertexPositionColor>* primitiveBatch)
{
    VertexPositionColor pos;

    VertexPositionColor velocity(m_position + m_velocity, Colors::Green);
    VertexPositionColor acceleration(m_position + m_acceleration, Colors::Red);

    pos = VertexPositionColor(m_position, Colors::Green);
    primitiveBatch->DrawLine(pos, velocity);
    pos = VertexPositionColor(m_position, Colors::Red);
    primitiveBatch->DrawLine(pos, acceleration);

    // Render debug info from behavior modules.
    for (const auto& behaviorModuleMapPair : m_behaviorModules)
    {
        auto behaviorModule = behaviorModuleMapPair.second;
        behaviorModule->RenderDebugInfo(primitiveBatch);
    }
}

void GameObject::AddBehaviorModule(std::shared_ptr<BehaviorModule> behaviorModule)
{
    AddBehaviorModule(behaviorModule, behaviorModule->GetDefaultPriorityLevel());
}

void GameObject::AddBehaviorModule(std::shared_ptr<BehaviorModule> behaviorModule, char priority)
{
    m_behaviorModules.insert(std::pair<char, std::shared_ptr<BehaviorModule>>(priority, behaviorModule));
}

void GameObject::CreateTexture(ID3D11Device2* device)
{
    ComPtr<ID3D11Resource> resource;
    DX::ThrowIfFailed(
        CreateWICTextureFromFile(device, GameObject_DefaultTextureFile, resource.GetAddressOf(), m_texture.ReleaseAndGetAddressOf())
    );

    ComPtr<ID3D11Texture2D> texture;
    DX::ThrowIfFailed(resource.As(&texture));

    CD3D11_TEXTURE2D_DESC textureDesc;
    texture->GetDesc(&textureDesc);

    m_textureOrigin.x = float(textureDesc.Width / 2);
    m_textureOrigin.y = float(textureDesc.Height / 2);

    // Calculate inertia based on object mass and texture width (assume circular shape)
    // TODO: also use Shape to calculate inertia
    auto radius = m_textureOrigin.x;
    m_inertia = 0.5f * m_mass * radius * radius;
}

void GameObject::ResetTexture()
{
    m_texture.Reset();
}

void GameObject::AddForceAtPosition(Vector2 force, Vector2 position)
{
    AddForce(force);
    AddTorque(position.Cross(force).Length());
}

void GameObject::AddImpulseAtPosition(Vector2 impulse, Vector2 position)
{
    m_velocity += impulse * m_mass;
    m_speed = m_velocity.Length();
    m_angularVelocity += position.Cross(impulse).Length() * m_inertia;
}
