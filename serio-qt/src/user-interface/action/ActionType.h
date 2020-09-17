#ifndef SERIO_ACTIONTYPE_H
#define SERIO_ACTIONTYPE_H

#include <QObject>

namespace serio::qt {

Q_NAMESPACE
enum ActionType {
    LOAD_ALL_TV_SHOWS_LIST_PAGE,
    LOAD_WATCHED_TV_SHOWS_LIST_PAGE,
    SET_TV_SHOW_NAME,
    OPEN_TV_SHOW_CRAWLER_EDITOR_VIEW,
    LOAD_TV_SHOW_CRAWLER_EDITOR_TV_SHOW_NAME,
    SAVE_TV_SHOW_CRAWLER,
    SAVE_TV_SHOW_CRAWLER_WITH_OVERRIDE,
    OPEN_CRAWLER_EDITOR,
    LOAD_CRAWLER_STEPS,
    SAVE_CRAWLER,
    OPEN_NEW_CRAWLER_STEP_EDITOR,
    OPEN_EXISTING_CRAWLER_STEP_EDITOR,
    LOAD_CRAWLER_STEP,
    SELECT_CRAWLER_STEP_TYPE,
    SAVE_CRAWLER_STEP,
    REMOVE_CRAWLER_STEP,
    BACK
};
Q_ENUMS(ActionType)

}

#endif //SERIO_ACTIONTYPE_H
