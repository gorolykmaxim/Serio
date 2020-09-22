#ifndef SERIO_CRAWLRESULT_H
#define SERIO_CRAWLRESULT_H

#include <tv-show-crawler-runtime/model/CrawlLogEntry.h>
#include <vector>

namespace serio::core {

struct CrawlResult {
    std::vector<CrawlLogEntry> log;
    std::vector<std::string> result;
};

}

#endif //SERIO_CRAWLRESULT_H
