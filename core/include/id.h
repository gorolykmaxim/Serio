#ifndef SERIO_ID_H
#define SERIO_ID_H

typedef unsigned int id;

struct id_seed {
    id current_value = 1;
};

id create_id(id_seed& seed);

#endif //SERIO_ID_H
