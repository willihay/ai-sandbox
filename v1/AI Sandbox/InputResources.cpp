#include "pch.h"
#include "InputResources.h"

using namespace DirectX;

InputResources::InputResources()
{
}

InputResources::~InputResources()
{
}

void InputResources::CreateInputResources(IUnknown* window)
{
    m_keyboard = std::make_unique<Keyboard>();
    m_keyboard->SetWindow(reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(window));

    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(window));
}

void InputResources::Resume()
{
    m_keyboardTracker.Reset();
    m_mouseTracker.Reset();
}

void InputResources::Update()
{
    auto kbState = m_keyboard->GetState();
    m_keyboardTracker.Update(kbState);

    auto mouseState = m_mouse->GetState();
    m_mouseTracker.Update(mouseState);
}
