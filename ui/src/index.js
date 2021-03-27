import ReactDOM from 'react-dom';
import "./index.css";
import UserInterface from "./UserInterface";
import Logo from "./view/Logo";
import Settings from "./view/Settings";
import Dialog from "./common/Dialog";
import EditTextDialog from "./view/EditTextDialog";
import SelectDialog from "./view/SelectDialog";
import Search from "./view/Search";
import {CoreStub} from "./development/CoreStub";

const coreStub = new CoreStub();
const userInterface = new UserInterface(coreStub.sendEvent.bind(coreStub), Logo);
window.userInterface = userInterface;
userInterface.registerView(1, EditTextDialog);
userInterface.registerView(2, Settings);
userInterface.registerView(3, Dialog);
userInterface.registerView(4, SelectDialog);
userInterface.registerView(5, Search);

ReactDOM.render(
    userInterface.render(),
    document.getElementById('root')
);
coreStub.start();