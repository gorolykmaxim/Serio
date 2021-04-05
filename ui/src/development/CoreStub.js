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
    state: "loaded",
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
    state: "loaded",
    emptyGridPlaceholderText: "No TV Shows Found :("
};
const preSearchEvent = {viewId: 5, searchBar, cardGrid: {}};
const loadingSearchEvent = {viewId: 5, searchBar, cardGrid: {state: "loading"}};
const searchEvent = {viewId: 5, searchBar, cardGrid: emptyCardGrid};
const searchEventWithTvShows = {viewId: 5, searchBar, cardGrid};

export class CoreStub {
    constructor() {
        this.incomingToSendOutgoing = {};
        this.incomingToSendOutgoing[settingsEvent.list.items[0].selectEvent.event] = this._sendSequence(setCrawlerConfigUrlEvent);
        this.incomingToSendOutgoing[settingsEvent.list.items[1].selectEvent.event] = this._sendSequence(selectLanguageEvent);
        this.incomingToSendOutgoing[settingsEvent.list.items[2].selectEvent.event] = this._sendSequence(clearCacheEvent);
        this.incomingToSendOutgoing[settingsEvent.appBar.backEvent.event] = this._sendSequence(preSearchEvent);
        this.incomingToSendOutgoing[setCrawlerConfigUrlEvent.dialog.confirmEvent.event] = this._sendSequence(settingsEvent);
        this.incomingToSendOutgoing[setCrawlerConfigUrlEvent.editText.valueChangeEvent.event] = this._sendSequence(setCrawlerConfigUrlEvent);
        this.incomingToSendOutgoing[selectLanguageEvent.list.items[0].selectEvent.event] = this._sendSequence(settingsEvent);
        this.incomingToSendOutgoing[clearCacheEvent.dialog.confirmEvent.event] = this._sendSequence(settingsEvent);
        this.incomingToSendOutgoing[searchEvent.searchBar.searchEvent.event] = this._sendSequence(searchEvent, loadingSearchEvent, searchEventWithTvShows);
        this.incomingToSendOutgoing[searchEvent.searchBar.backEvent.event] = this._sendSequence(settingsEvent);
    }
    start() {
        setTimeout(() => {
            window.userInterface.displayView(preSearchEvent);
        }, 1000);
    }
    sendEvent(event) {
        console.log(event);
        const sendOutgoing = this.incomingToSendOutgoing[event.event];
        if (sendOutgoing) {
            sendOutgoing();
        }
    }
    _sendSequence() {
        return () => {
            for (let i = 0; i < arguments.length; i++) {
                setTimeout(() => window.userInterface.displayView(arguments[i]), i * 1000);
            }
        };
    }
}