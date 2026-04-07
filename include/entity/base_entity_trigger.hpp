#include "entity/base_entity.hpp"

class base_entity_trigger : public base_entity {
    public:
        virtual void b_on_enter(base_entity* activator);

        virtual void b_on_exit(base_entity* activator);

        virtual ~base_entity_trigger();
    
    protected:
        base_entity_trigger();

        bool enabled;
};