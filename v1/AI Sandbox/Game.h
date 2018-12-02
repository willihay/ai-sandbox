//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "InputResources.h"
#include "StepTimer.h"
#include "World.h"

// A basic game implementation that creates a D3D11 device and provides a game loop.
class Game : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);

    // Initialization and management
    void Initialize(IUnknown* window, int width, int height, DXGI_MODE_ROTATION rotation);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height, DXGI_MODE_ROTATION rotation);
    void ValidateDevice();

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // I/O resources
    std::unique_ptr<InputResources>         m_inputResources;

    // Rendering resources
    std::unique_ptr<DirectX::BasicEffect>       m_basicEffect;
    std::unique_ptr<DirectX::CommonStates>      m_commonStates;
    std::unique_ptr<DX::DeviceResources>        m_deviceResources;
    std::unique_ptr<DirectX::SpriteFont>        m_fontDebugInfo;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>   m_inputLayout;
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  m_primitiveBatch;
    bool                                        m_showDebugInfo;
    std::unique_ptr<DirectX::SpriteBatch>       m_spriteBatch;
    DX::StepTimer                               m_timer;

    // World
    std::unique_ptr<World>                  m_world;
};