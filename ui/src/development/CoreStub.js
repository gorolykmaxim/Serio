const setCrawlerConfigUrlEvent = {
    viewId: 1,
    dialog: {
        title: "Crawler config URL",
        description: "Specify URL to the file, that contains configuration of all the crawlers, responsible for crawling tv shows.",
        cancelText: "Cancel",
        confirmText: "Save",
        cancelEvent: {
            event: "back-to-settings"
        },
        confirmEvent: {
            event: "crawler-config-url-save"
        }
    },
    editText: {
        value: "https://github.com/gorolykmaxim/content.json",
        label: "Crawler config URL",
        valueChangeEvent: {
            event: "crawler-config-url-changed"
        },
        saveValueEvent: {
            event: "crawler-config-url-save"
        }
    }
};
const settingsEvent = {
    viewId: 2,
    appBar: {
        title: "Settings",
        backEvent: {
            event: "back-to-search"
        },
    },
    list: {
        items: [
            {
                primaryText: "Crawler config URL",
                secondaryText: "https://github.com/gorolykmaxim/tv-show-content",
                icon: "Dns",
                selectEvent: {
                    event: "set-crawler-config-url"
                }
            },
            {
                primaryText: "Language",
                secondaryText: "English",
                icon: "Language",
                selectEvent: {
                    event: "select-language"
                }
            },
            {
                primaryText: "Clear cache",
                icon: "Delete",
                selectEvent: {
                    event: "ask-clear-cache-confirmation"
                }
            },
        ]
    }
};
const clearCacheEvent = {
    viewId: 3,
    dialog: {
        title: "Clear cache",
        description: "You are about to clear all the cache. This might temporarily decrease application's performance. Are you sure you want to do that?",
        cancelText: "Cancel",
        confirmText: "Confirm",
        cancelEvent: {
            event: "back-to-settings"
        },
        confirmEvent: {
            event: "clear-cache"
        }
    }
};
const selectLanguageEvent = {
    viewId: 4,
    appBar: {
        title: "Choose language",
        backEvent: {
            event: "back-to-settings"
        },
    },
    list: {
        selected: 1,
        items: [
            {
                primaryText: "English",
                selectEvent: {event: "language-select"}
            },
            {
                primaryText: "Русский",
                selectEvent: {event: "language-select"}
            }
        ]
    }
};
const searchBar = {
    placeholder: "Search",
    initialValue: "",
    valueChangeEvent: {
        event: "search-string-change"
    },
    searchEvent: {
        event: "search"
    },
    backEvent: {
        event: "back-to-settings"
    }
};
const emptyCardGrid = {
    items: [],
    selected: 0,
    emptyGridPlaceholderText: "No TV Shows Found :("
};
const cardGrid = {
    items: Array(50).fill({
        primaryText: "Boruto very-very-very-very-very-very-very-very-very long title",
        secondaryText: "2348723648723563278547132649124 days ago",
        image: "https://thumbs.filmix.ac/posters/orig/boruto-2017_117605_0.jpg",
        selectEvent: {
            event: "select-tv-show",
            id: "Boruto"
        }
    }).concat(Array(50).fill({
        primaryText: "Boruto very-very-very-very-very-very-very-very-very long title",
        image: "https://thumbs.filmix.ac/posters/orig/boruto-2017_117605_0.jpg",
        selectEvent: {
            event: "select-tv-show",
            id: "Boruto"
        }
    })),
    selected: 0,
    emptyGridPlaceholderText: "No TV Shows Found :("
};
const searchEvent = {viewId: 5, searchBar, cardGrid: emptyCardGrid};
const searchEventWithTvShows = {viewId: 5, searchBar, cardGrid};

export class CoreStub {
    constructor() {
        this.incomingToOutgoing = {};
        this.incomingToOutgoing[settingsEvent.list.items[0].selectEvent.event] = setCrawlerConfigUrlEvent;
        this.incomingToOutgoing[settingsEvent.list.items[1].selectEvent.event] = selectLanguageEvent;
        this.incomingToOutgoing[settingsEvent.list.items[2].selectEvent.event] = clearCacheEvent;
        this.incomingToOutgoing[settingsEvent.appBar.backEvent.event] = searchEvent;
        this.incomingToOutgoing[setCrawlerConfigUrlEvent.dialog.confirmEvent.event] = settingsEvent;
        this.incomingToOutgoing[setCrawlerConfigUrlEvent.editText.valueChangeEvent.event] = setCrawlerConfigUrlEvent;
        this.incomingToOutgoing[selectLanguageEvent.list.items[0].selectEvent.event] = settingsEvent;
        this.incomingToOutgoing[clearCacheEvent.dialog.confirmEvent.event] = settingsEvent;
        this.incomingToOutgoing[searchEvent.searchBar.searchEvent.event] = searchEventWithTvShows;
        this.incomingToOutgoing[searchEvent.searchBar.backEvent.event] = settingsEvent;
    }
    start() {
        setTimeout(() => {
            window.userInterface.displayView(searchEvent);
        }, 1000);
    }
    sendEvent(event) {
        console.log(event);
        event = this.incomingToOutgoing[event.event];
        window.userInterface.displayView(event);
    }
}