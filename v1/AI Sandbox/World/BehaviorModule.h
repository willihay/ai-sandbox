#pragma once

class GameObject;
class World;

class BehaviorModule
{
public:
    BehaviorModule();
    virtual ~BehaviorModule();

    // Override functions
    virtual char GetDefaultPriorityLevel() const { return Config::BehaviorModule_DefaultPriorityLevel; }
    virtual void RenderDebugInfo(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* primitiveBatch);
    virtual void Run(World* world, GameObject* object, float elapsedTime) = 0;

    // Base class functions
    bool IsEnabled() const { return m_enabled; }
    void SetEnabled(bool enabled) { m_enabled = enabled; }

private:
    bool m_enabled;
};
