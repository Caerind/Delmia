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
        void movePlacement(sf::Vector2i const& coords);
        void handleUnit(sf::Event const& event);

        void handleViewMovement(sf::Time dt);

        void onDeactivate();

    protected:
        World mWorld;

        bool mPlacing;
        bool mPlacingCollide;
        int mPlacingType;
        Building::Ptr mPlacement;

        bool mSelecting;
        sf::RectangleShape mSelectionZone;
        std::vector<Unit::Ptr> mSelectedUnits;

        sf::Vector2i oldC;
};


#endif // GAMESTATE_HPP
