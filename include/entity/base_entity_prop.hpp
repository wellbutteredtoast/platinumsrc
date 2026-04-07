#include "entity/base_entity.hpp"

class base_entity_prop : public base_entity {
    public:
        virtual void b_update(float delta);

        virtual ~base_entity_prop();

    protected:
        base_entity_prop();

        bool solid;     // collision flag
        bool visible;   // visible to world/renderer
};