import {Box, Container, List, makeStyles, Paper} from "@material-ui/core";
import * as Icons from "@material-ui/icons";
import ApplicationBar from "../common/ApplicationBar";
import SettingItem from "./SettingItem";
import RadioGroupDialog from "./RadioGroupDialog";
import {createElement, useState} from "react";

const useStyles = makeStyles(theme => ({
    root: {
        marginTop: theme.spacing(2)
    }
}));

function Settings() {
    const classes = useStyles();
    const [languageDialogOpen, setLanguageDialogOpen] = useState(false);
    const openLanguageDialog = () => setLanguageDialogOpen(true);
    const closeLanguageDialog = () => setLanguageDialogOpen(false);
    return (
        <Box>
            <ApplicationBar title="Settings" icon={<Icons.ArrowBack/>}/>
            <Container maxWidth="sm">
                <Paper className={classes.root}>
                    <List>
                        <SettingItem name="Crawler config URL"
                                     value="https://github.com/gorolykmaxim/tv-show-content"
                                     icon={createElement(Icons["Dns"])}
                                     autoFocus/>
                        <SettingItem name="Language"
                                     value="English"
                                     icon={createElement(Icons["Language"])}
                                     onClick={openLanguageDialog}/>
                        <SettingItem name="Clear cache"
                                     icon={createElement(Icons["Delete"])}/>
                    </List>
                </Paper>
            </Container>
            <RadioGroupDialog open={languageDialogOpen}
                              title="Choose language"
                              saveText="Save"
                              cancelText="Cancel"
                              value="English"
                              options={["English", "Русский"]}
                              onSave={closeLanguageDialog}
                              onCancel={closeLanguageDialog}/>
        </Box>
    );
}

export default function create() {
    return <Settings/>;
}