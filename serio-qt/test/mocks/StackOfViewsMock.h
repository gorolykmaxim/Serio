#ifndef SERIO_STACKOFVIEWSMOCK_H
#define SERIO_STACKOFVIEWSMOCK_H

#include <gmock/gmock.h>
#include <user-interface/StackOfViews.h>

class StackOfViewsMock : public serio::qt::StackOfViews {
public:
    MOCK_METHOD(void, pushView, (const QString&), (override));
    MOCK_METHOD(void, popCurrentView, (), (override));
    MOCK_METHOD(void, popAllViews, (), (override));
    MOCK_METHOD(void, popAllViewsUntil, (const QString&), (override));
    MOCK_METHOD(void, replaceCurrentViewWith, (const QString&), (override));
    MOCK_METHOD(void, replaceSpecifiedViewWith, (const QString&, const QString&), (override));
};

#endif //SERIO_STACKOFVIEWSMOCK_H
