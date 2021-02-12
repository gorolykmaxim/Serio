import {Box, Container, makeStyles, Paper} from "@material-ui/core";
import ChildAppBar from "../common/ChildAppBar";
import FocusableList from "../common/FocusableList";

const useStyles = makeStyles(theme => ({
    root: {
        marginTop: theme.spacing(2)
    }
}));

function Settings() {
    const classes = useStyles();
    const items = [
        {
            primaryText: "Crawler config URL",
            secondaryText: "https://github.com/gorolykmaxim/tv-show-content",
            icon: "Dns"
        },
        {
            primaryText: "Language",
            secondaryText: "English",
            icon: "Language"
        },
        {
            primaryText: "Clear cache",
            icon: "Delete"
        },
    ];
    return (
        <Box>
            <ChildAppBar title="Settings"/>
            <Container maxWidth="sm">
                <Paper className={classes.root}>
                    <FocusableList autoFocus items={items} onSelect={console.log}/>
                </Paper>
            </Container>
        </Box>
    );
}

export default function create() {
    return <Settings/>;
}