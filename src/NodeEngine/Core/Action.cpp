#include "Action.hpp"

NAction::NAction()
{
}

NAction::NAction(sf::Mouse::Button button, int type)
{
    setMouseButton(button,type);
}

NAction::NAction(sf::Keyboard::Key key, int type)
{
    setKeyboardKey(key,type);
}

NAction::NAction(sf::Event::EventType const& type)
{
    setEventType(type);
}

void NAction::setMouseButton(sf::Mouse::Button button, int type)
{
    mEvent.mouseButton.button = button;
    mEvent.type = sf::Event::MouseButtonPressed;
    if (type == Type::Released || type == Type::ReleasedOnce)
    {
        mEvent.type = sf::Event::MouseButtonReleased;
    }
    mType = type;
}

sf::Mouse::Button NAction::getMouseButton() const
{
    if (isMouseAction())
    {
        return mEvent.mouseButton.button;
    }
    else
    {
        return sf::Mouse::ButtonCount;
    }
}

void NAction::setKeyboardKey(sf::Keyboard::Key key, int type)
{
    mEvent.key.code = key;
    mEvent.type = sf::Event::KeyPressed;
    if (type == Type::Released || type == Type::ReleasedOnce)
    {
        mEvent.type = sf::Event::KeyReleased;
    }
    mType = type;
}

sf::Keyboard::Key NAction::getKeyboardKey() const
{
    if (isKeyboardAction())
    {
        return mEvent.key.code;
    }
    return sf::Keyboard::Unknown;
}

void NAction::setEventType(sf::Event::EventType const& event)
{
    mEvent.type = event;
    mType = Type::PressedOnce;
}

sf::Event::EventType NAction::getEventType() const
{
    return mEvent.type;
}

void NAction::setType(int type)
{
    mType = type;
}

int NAction::getType() const
{
    return mType;
}

bool NAction::isMouseAction() const
{
    return (mEvent.type == sf::Event::MouseButtonPressed || mEvent.type == sf::Event::MouseButtonReleased);
}

bool NAction::isKeyboardAction() const
{
    return (mEvent.type == sf::Event::KeyPressed || mEvent.type == sf::Event::KeyReleased);
}

bool NAction::isRealtime() const
{
    return (mType == Type::Pressed || mType == Type::Released);
}

bool NAction::test() const
{
    if (isRealtime())
    {
        // Keyboard
        if ((mEvent.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(mEvent.key.code))
        || (mEvent.type == sf::Event::KeyReleased && !sf::Keyboard::isKeyPressed(mEvent.key.code)))
        {
            return true;
        }
        // Mouse
        if ((mEvent.type == sf::Event::MouseButtonPressed && sf::Mouse::isButtonPressed(mEvent.mouseButton.button))
        || (mEvent.type == sf::Event::MouseButtonReleased && !sf::Mouse::isButtonPressed(mEvent.mouseButton.button)))
        {
            return true;
        }
    }
    return false;
}

bool NAction::operator==(sf::Event const& event) const
{
    if (mEvent.type == event.type)
    {
        if (isKeyboardAction())
        {
            return (mEvent.key.code == event.key.code);
        }
        else if (isMouseAction())
        {
            return (mEvent.mouseButton.button == event.mouseButton.button);
        }
        return true;
    }
    return false;
}

bool NAction::operator==(NAction const& action) const
{
    return (mType == action.mType && action == mEvent);
}

std::string NAction::keyToString(sf::Keyboard::Key const& key)
{
    switch (key)
    {
        #define CASE1(identifier) case sf::Keyboard::identifier: return #identifier; break;
        CASE1(Unknown);
		CASE1(A);
		CASE1(B);
		CASE1(C);
		CASE1(D);
		CASE1(E);
		CASE1(F);
		CASE1(G);
		CASE1(H);
		CASE1(I);
		CASE1(J);
		CASE1(K);
		CASE1(L);
		CASE1(M);
		CASE1(N);
		CASE1(O);
		CASE1(P);
		CASE1(Q);
		CASE1(R);
		CASE1(S);
		CASE1(T);
		CASE1(U);
		CASE1(V);
		CASE1(W);
		CASE1(X);
		CASE1(Y);
		CASE1(Z);
		CASE1(Num0);
		CASE1(Num1);
		CASE1(Num2);
		CASE1(Num3);
		CASE1(Num4);
		CASE1(Num5);
		CASE1(Num6);
		CASE1(Num7);
		CASE1(Num8);
		CASE1(Num9);
		CASE1(Escape);
		CASE1(LControl);
		CASE1(LShift);
		CASE1(LAlt);
		CASE1(LSystem);
		CASE1(RControl);
		CASE1(RShift);
		CASE1(RAlt);
		CASE1(RSystem);
		CASE1(Menu);
		CASE1(LBracket);
		CASE1(RBracket);
		CASE1(SemiColon);
		CASE1(Comma);
		CASE1(Period);
		CASE1(Quote);
		CASE1(Slash);
		CASE1(BackSlash);
		CASE1(Tilde);
		CASE1(Equal);
		CASE1(Dash);
		CASE1(Space);
		CASE1(Return);
		CASE1(BackSpace);
		CASE1(Tab);
		CASE1(PageUp);
		CASE1(PageDown);
		CASE1(End);
		CASE1(Home);
		CASE1(Insert);
		CASE1(Delete);
		CASE1(Add);
		CASE1(Subtract);
		CASE1(Multiply);
		CASE1(Divide);
		CASE1(Left);
		CASE1(Right);
		CASE1(Up);
		CASE1(Down);
		CASE1(Numpad0);
		CASE1(Numpad1);
		CASE1(Numpad2);
		CASE1(Numpad3);
		CASE1(Numpad4);
		CASE1(Numpad5);
		CASE1(Numpad6);
		CASE1(Numpad7);
		CASE1(Numpad8);
		CASE1(Numpad9);
		CASE1(F1);
		CASE1(F2);
		CASE1(F3);
		CASE1(F4);
		CASE1(F5);
		CASE1(F6);
		CASE1(F7);
		CASE1(F8);
		CASE1(F9);
		CASE1(F10);
		CASE1(F11);
		CASE1(F12);
		CASE1(F13);
		CASE1(F14);
		CASE1(F15);
		CASE1(Pause);
		default: return "Unknown"; break;
    }
}

std::string NAction::mouseButtonToString(sf::Mouse::Button const& button)
{
    switch (button)
    {
        #define CASE2(identifier) case sf::Mouse::identifier: return #identifier; break;
        CASE2(Left);
		CASE2(Right);
		CASE2(Middle);
		CASE2(XButton1);
		CASE2(XButton2);
		default: return "Unknown"; break;
    }
}

sf::Keyboard::Key NAction::stringToKey(std::string const& str)
{
    #define CASE3(identifier) if (str == #identifier) { return sf::Keyboard::identifier; }
    CASE3(Unknown);
    CASE3(A);
    CASE3(B);
    CASE3(C);
    CASE3(D);
    CASE3(E);
    CASE3(F);
    CASE3(G);
    CASE3(H);
    CASE3(I);
    CASE3(J);
    CASE3(K);
    CASE3(L);
    CASE3(M);
    CASE3(N);
    CASE3(O);
    CASE3(P);
    CASE3(Q);
    CASE3(R);
    CASE3(S);
    CASE3(T);
    CASE3(U);
    CASE3(V);
    CASE3(W);
    CASE3(X);
    CASE3(Y);
    CASE3(Z);
    CASE3(Num0);
    CASE3(Num1);
    CASE3(Num2);
    CASE3(Num3);
    CASE3(Num4);
    CASE3(Num5);
    CASE3(Num6);
    CASE3(Num7);
    CASE3(Num8);
    CASE3(Num9);
    CASE3(Escape);
    CASE3(LControl);
    CASE3(LShift);
    CASE3(LAlt);
    CASE3(LSystem);
    CASE3(RControl);
    CASE3(RShift);
    CASE3(RAlt);
    CASE3(RSystem);
    CASE3(Menu);
    CASE3(LBracket);
    CASE3(RBracket);
    CASE3(SemiColon);
    CASE3(Comma);
    CASE3(Period);
    CASE3(Quote);
    CASE3(Slash);
    CASE3(BackSlash);
    CASE3(Tilde);
    CASE3(Equal);
    CASE3(Dash);
    CASE3(Space);
    CASE3(Return);
    CASE3(BackSpace);
    CASE3(Tab);
    CASE3(PageUp);
    CASE3(PageDown);
    CASE3(End);
    CASE3(Home);
    CASE3(Insert);
    CASE3(Delete);
    CASE3(Add);
    CASE3(Subtract);
    CASE3(Multiply);
    CASE3(Divide);
    CASE3(Left);
    CASE3(Right);
    CASE3(Up);
    CASE3(Down);
    CASE3(Numpad0);
    CASE3(Numpad1);
    CASE3(Numpad2);
    CASE3(Numpad3);
    CASE3(Numpad4);
    CASE3(Numpad5);
    CASE3(Numpad6);
    CASE3(Numpad7);
    CASE3(Numpad8);
    CASE3(Numpad9);
    CASE3(F1);
    CASE3(F2);
    CASE3(F3);
    CASE3(F4);
    CASE3(F5);
    CASE3(F6);
    CASE3(F7);
    CASE3(F8);
    CASE3(F9);
    CASE3(F10);
    CASE3(F11);
    CASE3(F12);
    CASE3(F13);
    CASE3(F14);
    CASE3(F15);
    CASE3(Pause);
    return sf::Keyboard::KeyCount;
}

sf::Mouse::Button NAction::stringToMouseButton(std::string const& str)
{
    #define CASE4(identifier) if (str == #identifier) { return sf::Mouse::identifier; }
    CASE4(Left);
    CASE4(Right);
    CASE4(Middle);
    CASE4(XButton1);
    CASE4(XButton2);
    return sf::Mouse::ButtonCount;
}
