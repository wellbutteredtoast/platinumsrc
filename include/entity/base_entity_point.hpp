#include "entity/base_entity.hpp"

// intentionally bare, since this is just a point in space
// subclasses bring in everything else
class base_entity_point : public base_entity {
    protected:
        base_entity_point();
};