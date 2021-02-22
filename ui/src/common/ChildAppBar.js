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
 * @param {{title: string, color: string, elevation: number, onBack: Function, children: Array<JSX.Element>}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function ChildAppBar(props) {
    return (
        <ThemeProvider theme={appBarTheme}>
            <AppBar position="sticky" color={props.color || "inherit"} elevation={props.elevation || 0}>
                <Toolbar>
                    <IconButton edge="start" onClick={props.onBack}>
                        <ArrowBack/>
                    </IconButton>
                    <Text variant="h6">{props.title}</Text>
                    <ThemeProvider theme={darkTheme}>
                        {props.children}
                    </ThemeProvider>
                </Toolbar>
            </AppBar>
        </ThemeProvider>
    );
}