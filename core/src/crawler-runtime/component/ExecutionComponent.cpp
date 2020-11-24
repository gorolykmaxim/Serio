#include "ExecutionComponent.h"

namespace serio {

ExecutionComponent::ExecutionComponent() : error(MJS_OK) {
    mjs = mjs_create();
    execution.mjs = nullptr;
    execution.done = false;
}

ExecutionComponent::~ExecutionComponent() {
    mjs_destroy(mjs);
}
}