import ReactDOM from 'react-dom';
import "./index.css";
import UserInterface from "./UserInterface";
import Logo from "./logo/Logo";
import Settings from "./settings/Settings";
import TextFieldDialog from "./settings/TextFieldDialog";

const userInterface = new UserInterface(console.log, Logo);
window.userInterface = userInterface;
userInterface.registerView(1, TextFieldDialog);
userInterface.registerView(2, Settings);

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
userInterface.displayView(setCrawlerConfigUrlEvent);