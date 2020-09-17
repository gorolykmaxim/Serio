#include "ViewModel.h"

void serio::qt::ViewModel::modifyModel(const std::function<void()>& modify) {
    QMetaObject::invokeMethod(this, std::move(modify));
}
