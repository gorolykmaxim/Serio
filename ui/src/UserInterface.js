import {StrictMode, Component, createElement} from "react";
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
    constructor(sendEvent, fallbackView) {
        this.sendEvent = sendEvent;
        this.idToView = {};
        this.fallbackView = fallbackView;
    }
    registerView(id, view) {
        this.idToView[id] = view;
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
    _createView(id, data) {
        const view = this.idToView[id] || this.fallbackView;
        return createElement(view, Object.assign(data, {sendEvent: this.sendEvent}));
    }
}