#ifndef SERIO_JSOBJECT_H
#define SERIO_JSOBJECT_H

#include <mjs.h>
#include <string>
#include <vector>

namespace serio {
class JsObject {
public:
    mjs* mjs;

    explicit JsObject(struct mjs *mjs);
    JsObject(struct mjs *mjs, const std::vector<JsObject>& values);
    JsObject(struct mjs *mjs, double value);
    JsObject(struct mjs *mjs, const std::string& value);
    JsObject(struct mjs *mjs, bool value);
    JsObject(struct mjs *mjs, mjs_val_t root);
    [[nodiscard]] bool isNullOrUndefined() const;
    [[nodiscard]] bool isArray() const;
    [[nodiscard]] JsObject get(const std::string& name) const;
    void set(const std::string& name, JsObject value);
    [[nodiscard]] JsObject get(uint32_t index) const;
    [[nodiscard]] uint32_t size() const;
    std::string stringify();
    operator std::string();
    operator double();
    operator bool();
    operator mjs_val_t();
private:
    mjs_val_t root;
};
}

#endif //SERIO_JSOBJECT_H
