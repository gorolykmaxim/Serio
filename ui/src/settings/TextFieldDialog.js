import {Button, Container, Grid, makeStyles, TextField, Typography} from "@material-ui/core";
import {useState} from "react";
import {maxHeight} from "../common/Styles";

const marginBottomStyle = makeStyles(theme => ({
    root: {
        marginBottom: theme.spacing(1)
    }
}));

/**
 * @param {{title: string, description: string, label: string, saveText: string, cancelText: string, value: string, onValueChange: Function, onCancel: Function, onSave: Function}} props
 * @returns {JSX.Element}
 */
function TextFieldDialog(props) {
    const height = maxHeight();
    const marginBottom = marginBottomStyle();
    const [value, setValue] = useState(props.value || "");
    const onValueChanged = e => {
        const v = e.target.value;
        setValue(v);
        props.onValueChange(v);
    };
    return (
        <Container classes={height} maxWidth="xs">
            <Grid container
                  classes={height}
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
                           value={value}
                           onChange={onValueChanged}
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

export default function create(data, sendEvent) {
    return (
        <TextFieldDialog
            title={data.title}
            description={data.description}
            label={data.label}
            value={data.value}
            onCancel={() => sendEvent(data.cancelEvent)}
            onSave={() => sendEvent(data.saveEvent)}
            onValueChange={(v) => sendEvent(Object.assign({value: v}, data.valueChangeEvent))}
            cancelText={data.cancelText}
            saveText={data.saveText}/>
    );
}