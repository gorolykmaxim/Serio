import {Container, List, makeStyles, Paper} from "@material-ui/core";
import * as Icons from "@material-ui/icons";
import ApplicationBar from "./ApplicationBar";
import SettingItem from "./SettingItem";
import RadioGroupDialog from "./RadioGroupDialog";
import ConfirmationDialog from "./ConfirmationDialog";
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
    const [clearCacheDialogOpen, setClearCacheDialogOpen] = useState(false);
    const openClearCacheDialog = () => setClearCacheDialogOpen(true);
    const closeClearCacheDialog = () => setClearCacheDialogOpen(false);
    return (
        <div>
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
                                     icon={createElement(Icons["Delete"])}
                                     onClick={openClearCacheDialog}/>
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
            <ConfirmationDialog open={clearCacheDialogOpen}
                                title="Clear cache"
                                description="You are about to clear all the cache. This might temporarily decrease application's performance. Are you sure you want to do that?"
                                cancelText="Cancel"
                                acceptText="Confirm"
                                onCancel={closeClearCacheDialog}
                                onAccept={closeClearCacheDialog}/>
        </div>
    );
}

export default function create() {
    return <Settings/>;
}