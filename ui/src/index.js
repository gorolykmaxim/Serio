import ReactDOM from 'react-dom';
import "./index.css";
import UserInterface from "./UserInterface";
import Logo from "./view/Logo";
import Settings from "./view/Settings";
import TextFieldDialog from "./view/TextFieldDialog";
import ConfirmationDialog from "./view/ConfirmationDialog";
import SelectDialog from "./view/SelectDialog";
import {CoreStub} from "./development/CoreStub";

const coreStub = new CoreStub();
const userInterface = new UserInterface(coreStub.sendEvent.bind(coreStub), Logo);
window.userInterface = userInterface;
userInterface.registerView(1, TextFieldDialog);
userInterface.registerView(2, Settings);
userInterface.registerView(3, ConfirmationDialog);
userInterface.registerView(4, SelectDialog);

ReactDOM.render(
    userInterface.render(),
    document.getElementById('root')
);
coreStub.start();