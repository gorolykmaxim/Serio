#include "id.h"

id_seed::id_seed() : current_value(1) {}

id create_id(id_seed &seed) {
    return seed.current_value++;
}
