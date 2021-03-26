import {Button, TextField} from "@material-ui/core";
import {useState} from "react";
import {marginBottom} from "../Styles";
import CenteredView from "../common/CenteredView";
import Text from "../common/Text";
import {callOnEnter} from "../common/BrowserEvents";

/**
 * @param {{title: string, description: string, label: string, confirmText: string, cancelText: string, value: string, valueChangeEvent: Object, cancelEvent: Object, confirmEvent: Object, sendEvent: Function}} props
 * @returns {JSX.Element}
 */
function TextFieldDialog(props) {
    const {title, description, label, confirmText, cancelText, valueChangeEvent, cancelEvent, confirmEvent, sendEvent} = props;
    const margin = marginBottom();
    const [value, setValue] = useState(props.value || "");
    const onValueChanged = e => {
        const v = e.target.value;
        setValue(v);
        sendEvent(Object.assign({value: v}, valueChangeEvent));
    };
    return (
        <CenteredView>
            <Text variant="h6"
                        classes={margin}>{title}</Text>
            <Text color="textSecondary"
                        classes={margin}>{description}</Text>
            <TextField autoFocus
                       fullWidth
                       label={label}
                       margin="dense"
                       value={value}
                       onChange={onValueChanged}
                       onKeyDown={callOnEnter(() => sendEvent(confirmEvent))}
                       classes={margin}/>
            <Button fullWidth
                    variant="contained"
                    color="primary"
                    onClick={() => sendEvent(confirmEvent)}
                    classes={margin}>{confirmText}</Button>
            <Button fullWidth
                    onClick={() => sendEvent(cancelEvent)}>{cancelText}</Button>
        </CenteredView>
    );
}

export default function create(data, sendEvent) {
    return (
        <TextFieldDialog sendEvent={sendEvent} {...data}/>
    );
}