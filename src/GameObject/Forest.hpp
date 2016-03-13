#ifndef FOREST_HPP
#define FOREST_HPP

#include "../GameCore/Building.hpp"

class Forest : public Building
{
    public:
        typedef std::shared_ptr<Forest> Ptr;

    public:
        Forest();

        void init(int x, int y, std::size_t size);
};

#endif // FOREST_HPP
