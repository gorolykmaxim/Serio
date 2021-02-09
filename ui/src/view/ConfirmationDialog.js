import {Button, Typography} from "@material-ui/core";
import {marginBottom} from "../Styles";
import CenteredView from "../common/CenteredView";

/**
 * @param {{title: string, description: string, confirmText: string, cancelText: string, onConfirm: Function, onCancel: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
function ConfirmationDialog(props) {
    const margin = marginBottom();
    return (
        <CenteredView>
            <Typography variant="h6"
                        classes={margin}>{props.title}</Typography>
            <Typography color="textSecondary"
                        classes={margin}>{props.description}</Typography>
            <Button fullWidth
                    variant="contained"
                    color="primary"
                    onClick={props.onConfirm}
                    classes={margin}>{props.confirmText}</Button>
            <Button fullWidth
                    autoFocus
                    onClick={props.onCancel}>{props.cancelText}</Button>
        </CenteredView>
    );
}

export default function create(data, sendEvent) {
    return (
        <ConfirmationDialog
            title={data.title}
            description={data.description}
            onCancel={() => sendEvent(data.cancelEvent)}
            onConfirm={() => sendEvent(data.confirmEvent)}
            cancelText={data.cancelText}
            confirmText={data.confirmText}/>
    );
}