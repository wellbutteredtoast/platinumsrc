//
// entity/base_entity.hpp -> Basic entity object to extend on
//
// Entities are weird and doing everything customized and heavy
// will probably not work in the long term, given how little RAM
// some test hardware could have!
//
// I figured we could use OOP, and all work off a shared basic entity
// that we can extend as we need to, to avoid code repetition.
//

#include "lm/mmath.h"
#include "lm/vmath.h"

// Remember to name base_entity funcs with b_ please!
class base_entity {
    public:
        virtual void b_init();

        virtual void b_update(float delta);
        
        virtual void b_destroy();

        lm_mat4 b_get_model_matrix() const;

        virtual ~base_entity();

        lm_vec3 position;
        lm_vec3 rotation;
        lm_vec3 scale;
        const char* classname;
    
    protected:
        base_entity();

    private:
        // non-copyable, entities are owned by the world
        base_entity(const base_entity&);
        base_entity& operator=(const base_entity&);

};