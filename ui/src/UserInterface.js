import {StrictMode, Component} from "react";
import {CssBaseline, ThemeProvider} from "@material-ui/core";
import theme from "./Theme";

const EVENT_TYPE = "core-event";

class RootView extends Component {
    constructor(props) {
        super(props);
        this.userInterface = props.userInterface;
        this.state = {};
        this.onEvent = this.onEvent.bind(this);
    }
    componentDidMount() {
        window.addEventListener(EVENT_TYPE, this.onEvent);
    }
    componentWillUnmount() {
        window.removeEventListener(EVENT_TYPE, this.onEvent);
    }
    onEvent(event) {
        this.setState(event.detail);
    }
    render() {
        return (
            <StrictMode>
                <ThemeProvider theme={theme}>
                    <CssBaseline/>
                    {this.userInterface._createView(this.state.viewId, this.state)}
                </ThemeProvider>
            </StrictMode>
        );
    }
}

export default class UserInterface {
    constructor(sendEvent, fallbackViewFactoryMethod) {
        this.sendEvent = sendEvent;
        this.viewIdToFactoryMethod = {};
        this.fallbackViewFactoryMethod = fallbackViewFactoryMethod;
    }
    registerView(viewId, factoryMethod) {
        this.viewIdToFactoryMethod[viewId] = factoryMethod;
    }
    displayView(viewInformation) {
        const event = new CustomEvent(EVENT_TYPE, {detail: viewInformation});
        window.dispatchEvent(event);
    }
    render() {
        return (
            <RootView userInterface={this}/>
        );
    }
    _createView(viewId, data) {
        const factoryMethod = this.viewIdToFactoryMethod[viewId];
        if (!factoryMethod) {
            return this.fallbackViewFactoryMethod(data, this.sendEvent);
        } else {
            return factoryMethod(data, this.sendEvent);
        }
    }
}