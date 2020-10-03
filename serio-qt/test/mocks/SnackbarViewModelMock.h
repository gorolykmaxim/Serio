#ifndef SERIO_SNACKBARVIEWMODELMOCK_H
#define SERIO_SNACKBARVIEWMODELMOCK_H

#include <gmock/gmock.h>
#include <user-interface/view-model/SnackbarViewModel.h>

class SnackbarViewModelMock : public serio::qt::SnackbarViewModel {
public:
    MOCK_METHOD(void, displayText, (const QString&), (override));
};

#endif //SERIO_SNACKBARVIEWMODELMOCK_H
