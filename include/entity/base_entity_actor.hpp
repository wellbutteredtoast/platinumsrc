#include "entity/base_entity.hpp"

class base_entity_actor : public base_entity {
    public:
        virtual void b_update(float delta);

        virtual void b_on_death();
        
        virtual void b_take_damage(float amount, base_entity* inflictor);
    
        bool b_is_alive() const { return health >= 0.0f; }
    
        virtual ~base_entity_actor();
    
    protected:
        base_entity_actor();
    
        float health;
        float max_health;
        lm_vec3 velocity;
};