import {AppBar, createMuiTheme, IconButton, ThemeProvider, Toolbar} from "@material-ui/core";
import {ArrowBack} from "@material-ui/icons";
import Text from "./Text";
import darkTheme from "../Theme";

const appBarTheme = createMuiTheme({
    ...darkTheme,
    palette: {
        ...darkTheme.palette,
        background: {
            default: "#000",
            paper: "#000"
        }
    }
});

/**
 * @param {{appBar: Object, sendEvent: Function, color: string, elevation: number, children: Array<JSX.Element>}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function ChildAppBar(props) {
    const {title, backEvent} = props.appBar;
    return (
        <ThemeProvider theme={appBarTheme}>
            <AppBar position="sticky" color={props.color || "inherit"} elevation={props.elevation || 0}>
                <Toolbar>
                    <IconButton edge="start" onClick={() => props.sendEvent(backEvent)}>
                        <ArrowBack/>
                    </IconButton>
                    <Text variant="h6">{title}</Text>
                    <ThemeProvider theme={darkTheme}>
                        {props.children}
                    </ThemeProvider>
                </Toolbar>
            </AppBar>
        </ThemeProvider>
    );
}