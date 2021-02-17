import {AppBar, IconButton, Toolbar} from "@material-ui/core";
import {ArrowBack} from "@material-ui/icons";
import Text from "./Text";

/**
 * @param {{title: string, color: string, elevation: number, onBack: Function, children: Array<JSX.Element>}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function ChildAppBar(props) {
    return (
        <AppBar position="sticky" color={props.color || "inherit"} elevation={props.elevation}>
            <Toolbar>
                <IconButton edge="start" onClick={props.onBack}>
                    <ArrowBack/>
                </IconButton>
                <Text variant="h6">{props.title}</Text>
                {props.children}
            </Toolbar>
        </AppBar>
    );
}