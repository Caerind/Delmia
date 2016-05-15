#ifndef CITIZEN_HPP
#define CITIZEN_HPP

#include "Unit.hpp"

#include "../Buildings/Building.hpp"
#include "../Resources/Resource.hpp"

class Citizen : public Unit
{
    public:
        typedef std::shared_ptr<Citizen> Ptr;

    public:
        Citizen();
        Citizen(Player* player, sf::Vector2f const& pos);

        void gatherResource(Resource* resource);
        void build(Building* building);

    protected:
        Resource* mResource;
        Building* mBuilding;
};

#endif // CITIZEN_HPP
