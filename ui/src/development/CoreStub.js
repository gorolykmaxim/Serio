const setCrawlerConfigUrlEvent = {
    viewId: 1,
    title: "Crawler config URL",
    description: "Specify URL to the file, that contains configuration of all the crawlers, responsible for crawling tv shows.",
    label: "Crawler config URL",
    value: "https://github.com/gorolykmaxim/content.json",
    cancelText: "Cancel",
    saveText: "Save",
    valueChangeEvent: {
        event: "crawler-config-url-changed"
    },
    cancelEvent: {
        event: "back-to-settings"
    },
    saveEvent: {
        event: "crawler-config-url-save"
    }
};
const settingsEvent = {
    viewId: 2,
    title: "Settings",
    backEvent: {
        event: "back-to-search"
    },
    settings: [
        {
            name: "Crawler config URL",
            value: "https://github.com/gorolykmaxim/tv-show-content",
            icon: "Dns",
            selectEvent: {
                event: "set-crawler-config-url"
            }
        },
        {
            name: "Language",
            value: "English",
            icon: "Language",
            selectEvent: {
                event: "select-language"
            }
        },
        {
            name: "Clear cache",
            icon: "Delete",
            selectEvent: {
                event: "ask-clear-cache-confirmation"
            }
        },
    ]
};
const clearCacheEvent = {
    viewId: 3,
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
};
const selectLanguageEvent = {
    viewId: 4,
    title: "Choose language",
    selectedValue: "Русский",
    values: ["English", "Русский"],
    cancelEvent: {
        event: "back-to-settings"
    },
    selectEvent: {
        event: "language-select"
    }
};

const baseSearchEvent = {
    viewId: 5,
    searchText: "Search",
    searchString: "",
    selected: 0,
    noTvShowsFoundText: "No TV Shows Found :(",
    searchStringChangeEvent: {
        event: "search-string-change"
    },
    searchEvent: {
        event: "search"
    },
    backEvent: {
        event: "back-to-settings"
    }
};

const searchEvent = Object.assign({tvShows: []}, baseSearchEvent);

const searchEventWithTvShows = Object.assign({
    tvShows: Array(50).fill({
        title: "Boruto very-very-very-very-very-very-very-very-very long title",
        lastWatched: "2348723648723563278547132649124 days ago",
        thumbnail: "https://thumbs.filmix.ac/posters/orig/boruto-2017_117605_0.jpg",
        selectTvShowEvent: {
            event: "select-tv-show",
            id: "Boruto"
        }
    }).concat(Array(50).fill({
        title: "Boruto very-very-very-very-very-very-very-very-very long title",
        thumbnail: "https://thumbs.filmix.ac/posters/orig/boruto-2017_117605_0.jpg",
        selectTvShowEvent: {
            event: "select-tv-show",
            id: "Boruto"
        }
    })),
}, baseSearchEvent);

export class CoreStub {
    constructor() {
        this.incomingToOutgoing = {};
        this.incomingToOutgoing[settingsEvent.settings[0].selectEvent.event] = setCrawlerConfigUrlEvent;
        this.incomingToOutgoing[settingsEvent.settings[1].selectEvent.event] = selectLanguageEvent;
        this.incomingToOutgoing[settingsEvent.settings[2].selectEvent.event] = clearCacheEvent;
        this.incomingToOutgoing[settingsEvent.backEvent.event] = searchEvent;
        this.incomingToOutgoing[setCrawlerConfigUrlEvent.saveEvent.event] = settingsEvent;
        this.incomingToOutgoing[setCrawlerConfigUrlEvent.valueChangeEvent.event] = setCrawlerConfigUrlEvent;
        this.incomingToOutgoing[selectLanguageEvent.selectEvent.event] = settingsEvent;
        this.incomingToOutgoing[clearCacheEvent.confirmEvent.event] = settingsEvent;
        this.incomingToOutgoing[searchEvent.searchEvent.event] = searchEventWithTvShows;
        this.incomingToOutgoing[searchEvent.backEvent.event] = settingsEvent;
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