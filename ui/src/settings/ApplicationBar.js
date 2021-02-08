import {AppBar, IconButton, Toolbar, Typography} from "@material-ui/core";

/**
 * @param {{title: string, icon: JSX.Element, autoFocus: boolean}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function ApplicationBar(props) {
    return (
        <AppBar position="sticky" color="inherit">
            <Toolbar>
                <IconButton edge="start" color="inherit" autoFocus={props.autoFocus}>
                    {props.icon}
                </IconButton>
                <Typography variant="h6">{props.title}</Typography>
            </Toolbar>
        </AppBar>
    );
}