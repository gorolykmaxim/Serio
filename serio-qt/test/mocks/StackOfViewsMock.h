#ifndef SERIO_STACKOFVIEWSMOCK_H
#define SERIO_STACKOFVIEWSMOCK_H

#include <gmock/gmock.h>
#include <user-interface/StackOfViews.h>

class StackOfViewsMock : public serio::qt::StackOfViews {
public:
    MOCK_METHOD(void, pushView, (const QString&), (const override));
    MOCK_METHOD(void, popCurrentView, (), (const override));
    MOCK_METHOD(void, popAllViews, (), (const override));
    MOCK_METHOD(void, popAllViewsUntil, (const QString&), (const override));
    MOCK_METHOD(void, replaceCurrentViewWith, (const QString&), (const override));
    MOCK_METHOD(void, replaceSpecifiedViewWith, (const QString&, const QString&), (const override));
};

#endif //SERIO_STACKOFVIEWSMOCK_H
