#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "../../Game.hpp"

#include "../../Game/Buildings/Buildings.hpp"

class GameState : public ah::State
{
    public:
        GameState(ah::StateManager& manager);

        bool handleEvent(sf::Event const& event);
        bool update(sf::Time dt);
        void render(sf::RenderTarget& target, sf::RenderStates states);

        void handleZoom(sf::Event const& event);
        void handlePlacement(sf::Event const& event);
        void handleSelection(sf::Event const& event);
        void handleUnit(sf::Event const& event);
        void handleViewMovement(sf::Time dt);

        float getCameraSpeed();
        float getCameraZoom();

        void onDeactivate();

    protected:
        World mWorld;

        bool mPlacing;
        int mPlacingType;
        Building::Ptr mPlacement;

        bool mSelecting;
        sf::RectangleShape mSelectionZone;
        std::vector<Unit::Ptr> mSelected;

        float mCameraSpeed;
};


#endif // GAMESTATE_HPP
