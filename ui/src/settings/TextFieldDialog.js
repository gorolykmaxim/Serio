import {Button, Container, Grid, makeStyles, TextField, Typography} from "@material-ui/core";

const maxHeightStyle = makeStyles(() => ({
    root: {
        height: "100%"
    }
}));

const marginBottomStyle = makeStyles(theme => ({
    root: {
        marginBottom: theme.spacing(1)
    }
}));

/**
 * @param {{title: string, description: string, label: string, saveText: string, cancelText: string, value: string, onValueChange: Function, onCancel: Function, onSave: Function}} props
 * @returns {JSX.Element}
 */
export default function TextFieldDialog(props) {
    const maxHeight = maxHeightStyle();
    const marginBottom = marginBottomStyle();
    return (
        <Container classes={maxHeight} maxWidth="xs">
            <Grid container
                  classes={maxHeight}
                  direction="column"
                  justify="center"
                  alignItems="center">
                <Typography variant="h6"
                            classes={marginBottom}>{props.title}</Typography>
                <Typography color="textSecondary"
                            classes={marginBottom}>{props.description}</Typography>
                <TextField autoFocus
                           fullWidth
                           label={props.label}
                           margin="dense"
                           value={props.value}
                           onChange={e => props.onValueChange(e.target.value)}
                           classes={marginBottom}/>
                <Button fullWidth
                        variant="contained"
                        color="primary"
                        onClick={props.onSave}
                        classes={marginBottom}>{props.saveText}</Button>
                <Button fullWidth
                        onClick={props.onCancel}>{props.cancelText}</Button>
            </Grid>
        </Container>
    );
}