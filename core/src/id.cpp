#include "id.h"

id create_id(id_seed &seed) {
    return seed.current_value++;
}
