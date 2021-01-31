#ifndef SERIO_ENGLISH_H
#define SERIO_ENGLISH_H

namespace serio {
const Language ENGLISH {
    "English", {
        {TextId::serio, "Serio"},
        {TextId::search, "Search"},
        {TextId::today, "Today"},
        {TextId::yesterday, "Yesterday"},
        {TextId::daysAgo, "{} days ago"},
        {TextId::monthsAgo, "{} months ago"},
        {TextId::yearsAgo, "{} years ago"},
        {TextId::settings, "Settings"}
    }
};
}

#endif //SERIO_ENGLISH_H
