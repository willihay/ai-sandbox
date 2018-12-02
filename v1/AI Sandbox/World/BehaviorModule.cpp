#include "pch.h"
#include "BehaviorModule.h"


BehaviorModule::BehaviorModule() :
    m_enabled(true)
{
}

BehaviorModule::~BehaviorModule()
{
}

void BehaviorModule::RenderDebugInfo(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>*)
{
}
