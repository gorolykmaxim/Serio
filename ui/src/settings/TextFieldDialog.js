import {
    Button,
    Dialog,
    DialogActions,
    DialogContent,
    DialogContentText,
    DialogTitle,
    TextField
} from "@material-ui/core";
import {useState} from "react";

/**
 * @param {{open: boolean, title: string, description: string, label: string, value: string, cancelText: string, saveText: string, onSave: Function, onCancel: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function TextFieldDialog(props) {
    const [value, setValue] = useState(props.value);
    const onSave = () => props.onSave(value);
    return (
        <Dialog open={props.open} onClose={props.onCancel}>
            <DialogTitle>{props.title}</DialogTitle>
            <DialogContent>
                <DialogContentText>{props.description}</DialogContentText>
                <form onSubmit={e => {e.preventDefault(); onSave();}}>
                    <TextField autoFocus
                               margin="dense"
                               label={props.label}
                               value={value}
                               onChange={e => setValue(e.target.value)}
                               fullWidth/>
                </form>
            </DialogContent>
            <DialogActions>
                <Button onClick={props.onCancel}>{props.cancelText}</Button>
                <Button onClick={onSave} variant="contained" color="primary">{props.saveText}</Button>
            </DialogActions>
        </Dialog>
    );
}