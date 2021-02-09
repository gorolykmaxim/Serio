import {makeStyles} from "@material-ui/core";

export const maxHeight = makeStyles(() => ({
    root: {
        height: "100%"
    }
}));

export const marginBottom = makeStyles(theme => ({
    root: {
        marginBottom: theme.spacing(1)
    }
}));