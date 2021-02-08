import {Button, Dialog, DialogActions, DialogContent, DialogContentText, DialogTitle} from "@material-ui/core";

/**
 * @param {{open: boolean, title: string, description: string, acceptText: string, cancelText: string, onAccept: Function, onCancel: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function ConfirmationDialog(props) {
    return (
        <Dialog open={props.open} onClose={props.onCancel}>
            <DialogTitle>{props.title}</DialogTitle>
            <DialogContent>
                <DialogContentText>{props.description}</DialogContentText>
            </DialogContent>
            <DialogActions>
                <Button onClick={props.onCancel} autoFocus>{props.cancelText}</Button>
                <Button onClick={props.onAccept} variant="contained" color="primary">{props.acceptText}</Button>
            </DialogActions>
        </Dialog>
    );
}