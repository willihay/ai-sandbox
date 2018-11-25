//
// InputResources.h - a wrapper for storing input state from various input devices (mouse, keyboard, etc.)
//

#pragma once

class InputResources
{
public:
    InputResources();
    ~InputResources();

    void CreateInputResources(IUnknown* window);
    const DirectX::Keyboard::KeyboardStateTracker& GetKeyboardTracker() { return m_keyboardTracker; }
    const DirectX::Mouse::ButtonStateTracker& GetMouseTracker() { return m_mouseTracker; }
    void Resume();
    void Update();

private:
    std::unique_ptr<DirectX::Keyboard> m_keyboard;
    DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
    std::unique_ptr<DirectX::Mouse> m_mouse;
    DirectX::Mouse::ButtonStateTracker m_mouseTracker;
};
