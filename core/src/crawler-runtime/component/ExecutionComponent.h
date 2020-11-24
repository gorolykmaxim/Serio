#ifndef SERIO_EXECUTIONCOMPONENT_H
#define SERIO_EXECUTIONCOMPONENT_H

#include <string>
#include <mjs.h>

namespace serio {
struct ExecutionComponent {
    struct mjs* mjs;
    struct mjs_execution execution;
    std::string code;
    mjs_val_t result;
    mjs_err_t error;

    ExecutionComponent();
    virtual ~ExecutionComponent();
};
}

#endif //SERIO_EXECUTIONCOMPONENT_H
