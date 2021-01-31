#ifndef SERIO_RUSSIAN_H
#define SERIO_RUSSIAN_H

namespace serio {
const Language RUSSIAN {
    "Русский", {
        {TextId::serio, "Серио"},
        {TextId::search, "Поиск"},
        {TextId::today, "Сегодня"},
        {TextId::yesterday, "Вчера"},
        {TextId::daysAgo, "{} дней назад"},
        {TextId::monthsAgo, "{} месяцев назад"},
        {TextId::yearsAgo, "{} лет назад"},
        {TextId::settings, "Настройки"}
    }
};
}

#endif //SERIO_RUSSIAN_H
