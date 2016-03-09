#ifndef NACTOR_HPP
#define NACTOR_HPP

#include <memory>
#include "Component.hpp"

class NActor : public NComponent
{
    public:
        NActor();

        typedef std::shared_ptr<NActor> Ptr;

        virtual void load(pugi::xml_node& node, std::string const& name = "");
        virtual void save(pugi::xml_node& node, std::string const& name = "");

        std::string getId() const;

    protected:
        std::string mId;
};

#endif // NACTOR_HPP
