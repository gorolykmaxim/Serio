#ifndef SERIO_SERIO_TEST_H
#define SERIO_SERIO_TEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "http-mock.h"
#include "test-lib.h"
#include "test-data.h"
#include "test-data-en.h"
#include "test-data-ru.h"

class functional_test: public testing::Test {
protected:
    virtual void SetUp() {
        http_mock = std::make_shared<testing::NiceMock<nf_client_mock>>();
        http_mock->mock_response(CONFIG_URL, CONFIG);
        http_mock->mock_response("", "", nativeformat::http::StatusCodeInvalid);
        http_mock->mock_response(WRONG_CONFIG_URL, WRONG_CONFIG, nativeformat::http::StatusCodeInternalServerError);
        http_mock->mock_response(BROKEN_CONFIG_URL, WRONG_CONFIG);
        core.nf_client = http_mock;
    }

    std::shared_ptr<testing::NiceMock<nf_client_mock>> http_mock;
    core core;
};

class localized_functional_test: public functional_test, public testing::WithParamInterface<expected_render_tasks> {};

#endif //SERIO_SERIO_TEST_H
