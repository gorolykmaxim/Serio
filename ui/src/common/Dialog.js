import {marginBottom} from "../Styles";
import CenteredView from "./CenteredView";
import Text from "./Text";
import {Button} from "@material-ui/core";

/**
 * @param {{dialog: Object, sendEvent: Function, children: Array<JSX.Element>}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function Dialog(props) {
    const {title, description, confirmText, cancelText, confirmEvent, cancelEvent} = props.dialog;
    const margin = marginBottom();
    return (
        <CenteredView>
            <Text variant="h6"
                  classes={margin}>{title}</Text>
            <Text color="textSecondary"
                  classes={margin}>{description}</Text>
            {props.children}
            <Button fullWidth
                    variant="contained"
                    color="primary"
                    onClick={() => props.sendEvent(confirmEvent)}
                    classes={margin}>{confirmText}</Button>
            <Button fullWidth
                    autoFocus
                    onClick={() => props.sendEvent(cancelEvent)}>{cancelText}</Button>
        </CenteredView>
    );
}