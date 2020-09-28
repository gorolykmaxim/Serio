#ifndef SERIO_DIALOGVIEWMODELMOCK_H
#define SERIO_DIALOGVIEWMODELMOCK_H

#include <gmock/gmock.h>
#include <user-interface/view-model/DialogViewModel.h>
#include "StackOfViewsMock.h"

class DialogViewModelMock : public serio::qt::DialogViewModel {
public:
    inline static StackOfViewsMock instance;
    DialogViewModelMock() : DialogViewModel(instance) {}
    MOCK_METHOD(void, display, (const serio::qt::DialogModel&), (override));
};

#endif //SERIO_DIALOGVIEWMODELMOCK_H
