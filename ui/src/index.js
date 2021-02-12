import ReactDOM from 'react-dom';
import "./index.css";
import UserInterface from "./UserInterface";
import Logo from "./view/Logo";
import Settings from "./view/Settings";
import TextFieldDialog from "./view/TextFieldDialog";
import ConfirmationDialog from "./view/ConfirmationDialog";
import SelectDialog from "./view/SelectDialog";

const userInterface = new UserInterface(console.log, Logo);
window.userInterface = userInterface;
userInterface.registerView(1, TextFieldDialog);
userInterface.registerView(2, Settings);
userInterface.registerView(3, ConfirmationDialog);
userInterface.registerView(4, SelectDialog);

ReactDOM.render(
    userInterface.render(),
    document.getElementById('root')
);

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
        event: "back"
    },
    saveEvent: {
        event: "crawler-config-url-save"
    }
};
const settingsEvent = {
    viewId: 2,
    title: "Settings",
    backEvent: {
        event: "back"
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
        event: "back"
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
        event: "back"
    },
    selectEvent: {
        event: "language-select"
    }
};
userInterface.displayView(settingsEvent);