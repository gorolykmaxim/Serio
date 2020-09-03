#ifndef SERIO_FETCHSTEPEXECUTOR_H
#define SERIO_FETCHSTEPEXECUTOR_H

#include <http-client/HttpClient.h>
#include "CrawlerStepExecutor.h"

namespace serio::core {

class FetchStepExecutor : public CrawlerStepExecutor {
public:
    FetchStepExecutor(HttpClient &httpClient);
    std::vector<std::string> execute(const CrawlerStep &step, const std::vector<std::string> &previousStepResults) const override;
private:
    HttpClient& httpClient;
};

}

#endif //SERIO_FETCHSTEPEXECUTOR_H
