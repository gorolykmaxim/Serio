import {Button, Dialog, DialogActions, DialogContent, DialogTitle, FormControl, Select} from "@material-ui/core";
import {useState} from "react";

/**
 * @param {{open: boolean, title: string, options: Array<string>, value: string, saveText: string, cancelText: string, onSave: Function, onCancel: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function RadioGroupDialog(props) {
    const [value, setValue] = useState(props.value);
    return (
        <Dialog open={props.open}>
            <DialogTitle>{props.title}</DialogTitle>
            <DialogContent dividers>
                <FormControl fullWidth>
                    <Select native
                            value={value}
                            onChange={e => setValue(e.target.value)}>
                        {props.options.map(option => (
                            <option value={option}>{option}</option>
                        ))}
                    </Select>
                </FormControl>
            </DialogContent>
            <DialogActions>
                <Button onClick={props.onCancel}>{props.cancelText}</Button>
                <Button onClick={() => props.onSave(value)} variant="contained" color="primary">{props.saveText}</Button>
            </DialogActions>
        </Dialog>
    );
}