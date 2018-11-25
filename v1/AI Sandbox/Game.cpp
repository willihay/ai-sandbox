//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "RandomHelper.h"

// Behavior modules
#include "FollowBehavior.h"
#include "PlayerInput.h"

extern void ExitGame();

using namespace Config;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Helper;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false) :
    m_showDebugInfo(true)
{
    RandomInit();
    LoadConfigFile();

    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);

    m_inputResources = std::make_unique<InputResources>();

    m_world = std::make_unique<World>();
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(IUnknown* window, int width, int height, DXGI_MODE_ROTATION rotation)
{
    m_deviceResources->SetWindow(window, width, height, rotation);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    m_inputResources->CreateInputResources(window);

    // 60 FPS fixed timestep update logic
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);

    auto viewport = m_deviceResources->GetScreenViewport();

    // Setup world and its initial teams and players
    m_world->SetWorldBoundary(Vector2(viewport.Width, viewport.Height));
    m_world->CreateTeam();

    auto humanPlayer = std::make_shared<GameObject>(Vector2(viewport.Width / 2.f, viewport.Height / 2.f));
    humanPlayer->CreateTexture(m_deviceResources->GetD3DDevice());
    auto playerInputModule = std::make_shared<PlayerInput>(m_inputResources.get());
    humanPlayer->AddBehaviorModule(playerInputModule);
    m_world->AddPlayer(humanPlayer, 0);

    // Create (empty) second team, for AI agents
    m_world->CreateTeam();
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    float elapsedTime = float(timer.GetElapsedSeconds());

    m_inputResources->Update();

    auto kbTracker = m_inputResources->GetKeyboardTracker();
    auto mouseTracker = m_inputResources->GetMouseTracker();
    using ButtonState = Mouse::ButtonStateTracker::ButtonState;

    if (kbTracker.pressed.Escape)
    {
        ExitGame();
    }

    if (kbTracker.pressed.Space)
    {
        // Create new agent.
        auto viewport = m_deviceResources->GetScreenViewport();
        auto agent = std::make_shared<GameObject>(RandomScreenPosition(viewport));
        agent->CreateTexture(m_deviceResources->GetD3DDevice());
        agent->SetTextureTint(Colors::Red.v);
        auto followModule = std::make_shared<FollowBehavior>(m_world->GetPlayer(0, 0));
        agent->AddBehaviorModule(followModule);
        m_world->AddPlayer(agent, 1);
    }

    if (kbTracker.pressed.OemTilde)
    {
        // Toggle debug info display.
        m_showDebugInfo = !m_showDebugInfo;
    }

    m_world->Update(elapsedTime);

    if (mouseTracker.leftButton == ButtonState::PRESSED)
    {
        if (!m_world->GetPlayer(0, 0))
        {
            // No player on team 0...create one that's human-controlled!
            auto viewport = m_deviceResources->GetScreenViewport();
            auto humanPlayer = std::make_shared<GameObject>(Vector2(viewport.Width / 2.f, viewport.Height / 2.f));
            humanPlayer->CreateTexture(m_deviceResources->GetD3DDevice());
            auto playerInputModule = std::make_shared<PlayerInput>(m_inputResources.get());
            humanPlayer->AddBehaviorModule(playerInputModule);
            m_world->AddPlayer(humanPlayer, 0);
        }
    }
    else if (mouseTracker.middleButton == ButtonState::PRESSED)
    {
        m_world->RemoveAllPlayers(1); // remove all team 1 players
    }
    else if (mouseTracker.rightButton == ButtonState::PRESSED)
    {
        m_world->RemovePlayer(m_world->GetPlayer(0, 0)); // a test...
    }

    PIXEndEvent();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    auto context = m_deviceResources->GetD3DDeviceContext();
    PIXBeginEvent(context, PIX_COLOR_DEFAULT, L"Render");

    // Render SpriteBatch objects.
    m_spriteBatch->Begin();

    m_world->Render(m_spriteBatch.get());

    m_spriteBatch->End();

    // Render PrimitiveBatch objects.
    context->OMSetBlendState(m_commonStates->Opaque(), nullptr, 0xFFFFFFFF);
    context->OMSetDepthStencilState(m_commonStates->DepthNone(), 0);
    context->RSSetState(m_commonStates->CullNone());

    m_basicEffect->Apply(context);
    context->IASetInputLayout(m_inputLayout.Get());

    m_primitiveBatch->Begin();

    if (m_showDebugInfo)
    {
        for (const auto& player : m_world->GetTeam(0))
        {
            player->RenderDebugInfo(m_primitiveBatch.get());
        }
    }

    m_primitiveBatch->End();

    PIXEndEvent(context);

    // Show the new frame.
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Present");
    m_deviceResources->Present();
    PIXEndEvent();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    auto context = m_deviceResources->GetD3DDeviceContext();
    PIXBeginEvent(context, PIX_COLOR_DEFAULT, L"Clear");

    // Clear the views.
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::Black);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    PIXEndEvent(context);
 }
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    auto context = m_deviceResources->GetD3DDeviceContext();
    context->ClearState();

    m_deviceResources->Trim();

    // TODO: Game is being power-suspended.
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();
    m_inputResources->Resume();
}

void Game::OnWindowSizeChanged(int width, int height, DXGI_MODE_ROTATION rotation)
{
    if (!m_deviceResources->WindowSizeChanged(width, height, rotation))
        return;

    CreateWindowSizeDependentResources();

    m_world->SetWorldBoundary(Vector2(float(width), float(height)));
}

void Game::ValidateDevice()
{
    m_deviceResources->ValidateDevice();
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto device = m_deviceResources->GetD3DDevice();

    m_commonStates = std::make_unique<CommonStates>(device);

    m_basicEffect = std::make_unique<BasicEffect>(device);
    m_basicEffect->SetVertexColorEnabled(true);

    void const* shaderByteCode;
    size_t byteCodeLength;
    m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

    DX::ThrowIfFailed(
        device->CreateInputLayout(
            VertexPositionColor::InputElements,
            VertexPositionColor::InputElementCount,
            shaderByteCode,
            byteCodeLength,
            m_inputLayout.ReleaseAndGetAddressOf())
    );

    m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);
    m_spriteBatch = std::make_unique<SpriteBatch>(context);

    m_world->CreateAllTextures(device);
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    auto windowSize = m_deviceResources->GetOutputSize();

    Matrix proj = Matrix::CreateScale(
        2.f / float(windowSize.right),
        -2.f / float(windowSize.bottom),
        1.f) * Matrix::CreateTranslation(-1.f, 1.f, 0.f);

    m_basicEffect->SetProjection(proj);
}

void Game::OnDeviceLost()
{
    m_commonStates.reset();
    m_spriteBatch.reset();
    m_basicEffect.reset();
    m_inputLayout.Reset();

    m_world->ResetAllTextures();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
