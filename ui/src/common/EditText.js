import {callOnEnter} from "./BrowserEvents";
import {TextField} from "@material-ui/core";
import {marginBottom} from "../Styles";
import {useValue} from "../ValueState";

/**
 * @param {{editText: Object, sendEvent: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function EditText(props) {
    const {label, valueChangeEvent, saveValueEvent} = props.editText;
    const [value, onValueChanged] = useValue(props.editText.value, valueChangeEvent, props.sendEvent);
    return (
        <TextField autoFocus
                   fullWidth
                   label={label}
                   margin="dense"
                   value={value}
                   onChange={onValueChanged}
                   onKeyDown={callOnEnter(() => props.sendEvent(saveValueEvent))}
                   classes={marginBottom()}/>
    );
}