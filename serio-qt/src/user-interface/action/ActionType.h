#ifndef SERIO_ACTIONTYPE_H
#define SERIO_ACTIONTYPE_H

#include <QObject>

namespace serio::qt {

Q_NAMESPACE
enum ActionType {
    LOAD_ALL_TV_SHOWS_LIST_PAGE,
    LOAD_WATCHED_TV_SHOWS_LIST_PAGE,
    LOAD_FIRST_PAGE_OF_TV_SHOWS,
    SET_TV_SHOW_NAME,
    OPEN_ADD_TV_SHOW_VIEW,
    OPEN_TV_SHOW_CRAWLER_EDITOR_VIEW,
    LOAD_TV_SHOW_CRAWLER_EDITOR_TV_SHOW_NAME,
    SAVE_TV_SHOW_CRAWLER,
    SAVE_TV_SHOW_CRAWLER_WITH_OVERRIDE,
    OPEN_CRAWLER_EDITOR,
    OPEN_CRAWLER_EDITOR_HELP,
    LOAD_CRAWLER_STEPS,
    PREVIEW_CRAWLER,
    SAVE_CRAWLER,
    OPEN_NEW_CRAWLER_STEP_EDITOR,
    OPEN_EXISTING_CRAWLER_STEP_EDITOR,
    LOAD_CRAWLER_STEP,
    SELECT_CRAWLER_STEP_TYPE,
    SAVE_CRAWLER_STEP,
    REMOVE_CRAWLER_STEP,
    OPEN_PREVIEWED_CRAWLER_LOG,
    OPEN_IMPORT_TV_SHOW_VIEW,
    IMPORT_TV_SHOW_CRAWLER,
    OPEN_CRAWL_LOG_ENTRY_VIEW,
    OPEN_TV_SHOW_VIEW,
    LOAD_TV_SHOW,
    LOAD_EPISODES_LIST_PAGE,
    OPEN_CURRENT_TV_SHOW_CRAWLER_EDITOR_VIEW,
    SHARE_CRAWLER_OF_CURRENT_TV_SHOW,
    BACK
};
Q_ENUMS(ActionType)

}

#endif //SERIO_ACTIONTYPE_H
