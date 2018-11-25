#pragma once

class GameObject;
class World;

class BehaviorModule
{
public:
    BehaviorModule();
    virtual ~BehaviorModule();

    virtual void Run(World* world, GameObject* object, float elapsedTime) = 0;
    virtual void RenderDebugInfo(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitiveBatch);

    virtual char GetDefaultPriorityLevel() const { return Config::BehaviorModule_DefaultPriorityLevel; }
    bool IsEnabled() const { return m_enabled; }
    void SetEnabled(bool enabled) { m_enabled = enabled; }

private:
    bool m_enabled;
};
