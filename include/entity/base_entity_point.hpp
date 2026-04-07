#include "entity/base_entity.hpp"

class base_entity_point : public base_entity {
    // intentionally bare, since this is just a point in space
    // subclasses bring in everything else
    protected:
        base_entity_point();
};