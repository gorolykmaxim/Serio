import {AppBar, IconButton, Toolbar, Typography} from "@material-ui/core";
import {ArrowBack} from "@material-ui/icons";

/**
 * @param {{title: string}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function ChildAppBar(props) {
    return (
        <AppBar position="sticky" color="inherit">
            <Toolbar>
                <IconButton edge="start">
                    <ArrowBack/>
                </IconButton>
                <Typography variant="h6">{props.title}</Typography>
            </Toolbar>
        </AppBar>
    );
}