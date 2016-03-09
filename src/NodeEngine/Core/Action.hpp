#ifndef NACTION_HPP
#define NACTION_HPP

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

class NAction
{
    public:
        enum Type
        {
            None = 0,
            Pressed = 1,
            Released = 2,
            PressedOnce = 3,
            ReleasedOnce = 4,
        };

        NAction();
        NAction(sf::Mouse::Button button, int type);
        NAction(sf::Keyboard::Key key, int type);
        NAction(sf::Event::EventType const& type);

        void setMouseButton(sf::Mouse::Button button, int type);
        sf::Mouse::Button getMouseButton() const;

        void setKeyboardKey(sf::Keyboard::Key key, int type);
        sf::Keyboard::Key getKeyboardKey() const;

        void setEventType(sf::Event::EventType const& event);
        sf::Event::EventType getEventType() const;

        void setType(int type);
        int getType() const;

        bool isMouseAction() const;
        bool isKeyboardAction() const;

        bool isRealtime() const;

        bool test() const;

        bool operator==(sf::Event const& event) const;
        bool operator==(NAction const& action) const;

        static std::string keyToString(sf::Keyboard::Key const& key);
        static std::string mouseButtonToString(sf::Mouse::Button const& button);
        static sf::Keyboard::Key stringToKey(std::string const& str);
        static sf::Mouse::Button stringToMouseButton(std::string const& str);

    protected:
        sf::Event mEvent;
        int mType;
};

#endif // NACTION_HPP
