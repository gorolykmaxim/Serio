#include "JsObject.h"

namespace serio {
JsObject::JsObject(struct mjs *mjs) : mjs(mjs), root(mjs_mk_null()) {}

JsObject::JsObject(struct mjs *mjs, const std::vector<JsObject>& values) : mjs(mjs), root(mjs_mk_array(mjs)) {
    for (const auto& value: values) {
        mjs_array_push(mjs, root, value.root);
    }
}

JsObject::JsObject(struct mjs *mjs, double value) : mjs(mjs), root(mjs_mk_number(mjs, value)) {}

JsObject::JsObject(struct mjs *mjs, const std::string &value) : mjs(mjs), root(mjs_mk_string(mjs, value.c_str(), value.size(), true)) {}

JsObject::JsObject(struct mjs *mjs, bool value) : mjs(mjs), root(mjs_mk_boolean(mjs, value)) {}

JsObject::JsObject(struct mjs *mjs, mjs_val_t root) : mjs(mjs), root(root) {}

bool JsObject::isNullOrUndefined() const {
    return mjs_is_null(root) || mjs_is_undefined(root);
}

bool JsObject::isArray() const {
    return mjs_is_array(root);
}

JsObject JsObject::get(const std::string &name) const {
    const auto fieldName = mjs_mk_string(mjs, name.c_str(), name.size(), false);
    return JsObject(mjs, mjs_get_v(mjs, root, fieldName));
}

void JsObject::set(const std::string &name, JsObject value) {
    const auto fieldName = mjs_mk_string(mjs, name.c_str(), name.length(), true);
    mjs_set_v(mjs, root, fieldName, value.root);
}

JsObject JsObject::get(uint32_t index) const {
    return JsObject(mjs, mjs_array_get(mjs, root, index));
}

uint32_t JsObject::size() const {
    return mjs_array_length(mjs, root);
}

std::string JsObject::stringify() {
    char* resultPtr;
    if (mjs_json_stringify(mjs, root, nullptr, 0, &resultPtr) == MJS_OK) {
        std::string result(resultPtr);
        delete resultPtr;
        return result;
    } else {
        return "";
    }
}

JsObject::operator std::string() {
    return mjs_get_cstring(mjs, &root);
}

JsObject::operator double() {
    return mjs_get_double(mjs, root);
}

JsObject::operator bool() {
    return mjs_get_bool(mjs, root);
}

JsObject::operator mjs_val_t() {
    return root;
}
}