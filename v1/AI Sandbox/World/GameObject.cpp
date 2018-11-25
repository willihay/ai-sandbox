#include "pch.h"
#include "GameObject.h"
#include "WICTextureLoader.h"
#include "World.h"

using namespace Config;
using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

GameObject::GameObject(Vector2 position) :
    m_acceleration(Vector2::Zero),
    m_facingAngle(0.f),
    m_isValidTarget(true),
    m_maxAcceleration(GameObject_DefaultMaxAcceleration),
    m_maxAngularRotation(GameObject_DefaultMaxAngularRotation),
    m_maxSpeed(GameObject_DefaultMaxSpeed),
    m_movementCalculation(MovementCalculationType::MovementCalculation_AddAcceleration),
    m_position(position),
    m_speed(0.f),
    m_teamNumber(0),
    m_textureOrigin(Vector2::Zero),
    m_textureTint(Colors::White.v),
    m_velocity(Vector2::Zero)
{
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
    
    auto right = Vector2(-std::sinf(m_facingAngle), std::cosf(m_facingAngle));
    auto forward = Vector2(std::cosf(m_facingAngle), std::sinf(m_facingAngle));

    // Set acceleration.
    m_acceleration = m_accelerationAccumulated;
    m_accelerationAccumulated = Vector2::Zero;
    // TODO: apply drag?

    // Update velocity and facing angle.
    m_velocity += m_acceleration * elapsedTime;
    m_speed = m_velocity.Length();

    if (m_speed > m_maxSpeed)
    {
        m_velocity *= m_maxSpeed / m_speed;
    }

    if (m_speed > 0.f)
    {
        m_facingAngle = std::atan2f(m_velocity.y, m_velocity.x);
    }

    // Update position.
    m_position += m_velocity * elapsedTime;

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
}

void GameObject::Render(SpriteBatch* spriteBatch)
{
    spriteBatch->Draw(m_texture.Get(), m_position, nullptr, m_textureTint, m_facingAngle, m_textureOrigin);
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
}

void GameObject::ResetTexture()
{
    m_texture.Reset();
}
