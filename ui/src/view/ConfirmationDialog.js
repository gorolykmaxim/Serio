import {Button} from "@material-ui/core";
import {marginBottom} from "../Styles";
import CenteredView from "../common/CenteredView";
import Text from "../common/Text";

/**
 * @param {{title: string, description: string, confirmText: string, cancelText: string, confirmEvent: Object, cancelEvent: Object, sendEvent: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
function ConfirmationDialog(props) {
    const {title, description, confirmText, cancelText, confirmEvent, cancelEvent, sendEvent} = props;
    const margin = marginBottom();
    return (
        <CenteredView>
            <Text variant="h6"
                        classes={margin}>{title}</Text>
            <Text color="textSecondary"
                        classes={margin}>{description}</Text>
            <Button fullWidth
                    variant="contained"
                    color="primary"
                    onClick={() => sendEvent(confirmEvent)}
                    classes={margin}>{confirmText}</Button>
            <Button fullWidth
                    autoFocus
                    onClick={() => sendEvent(cancelEvent)}>{cancelText}</Button>
        </CenteredView>
    );
}

export default function create(data, sendEvent) {
    return (
        <ConfirmationDialog sendEvent={sendEvent} {...data}/>
    );
}