import {Box, Container, List, makeStyles, Paper} from "@material-ui/core";
import * as Icons from "@material-ui/icons";
import ChildAppBar from "../common/ChildAppBar";
import SettingItem from "./SettingItem";
import {createElement} from "react";

const useStyles = makeStyles(theme => ({
    root: {
        marginTop: theme.spacing(2)
    }
}));

function Settings() {
    const classes = useStyles();
    return (
        <Box>
            <ChildAppBar title="Settings"/>
            <Container maxWidth="sm">
                <Paper className={classes.root}>
                    <List>
                        <SettingItem name="Crawler config URL"
                                     value="https://github.com/gorolykmaxim/tv-show-content"
                                     icon={createElement(Icons["Dns"])}
                                     autoFocus/>
                        <SettingItem name="Language"
                                     value="English"
                                     icon={createElement(Icons["Language"])}/>
                        <SettingItem name="Clear cache"
                                     icon={createElement(Icons["Delete"])}/>
                    </List>
                </Paper>
            </Container>
        </Box>
    );
}

export default function create() {
    return <Settings/>;
}