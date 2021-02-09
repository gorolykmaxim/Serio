import {AppBar, IconButton, Toolbar, Typography} from "@material-ui/core";
import {ArrowBack} from "@material-ui/icons";

/**
 * @param {{title: string, color: string, elevation: number, onBack: Function}} props
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
                <Typography variant="h6">{props.title}</Typography>
            </Toolbar>
        </AppBar>
    );
}