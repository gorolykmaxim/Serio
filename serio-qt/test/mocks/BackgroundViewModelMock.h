#ifndef SERIO_BACKGROUNDVIEWMODELMOCK_H
#define SERIO_BACKGROUNDVIEWMODELMOCK_H

#include <gmock/gmock.h>
#include <user-interface/view-model/BackgroundViewModel.h>

class BackgroundViewModelMock : public serio::qt::BackgroundViewModel {
public:
    MOCK_METHOD(void, setImage, (const QString&), (override));
};

#endif //SERIO_BACKGROUNDVIEWMODELMOCK_H
