#include "ViewModel.h"

namespace serio::qt {

void ViewModel::modifyModel(const std::function<void()>& modify) {
    QMetaObject::invokeMethod(this, modify);
}

}