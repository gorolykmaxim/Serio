import {useState} from "react";
import {callOnEnter} from "./BrowserEvents";
import {TextField} from "@material-ui/core";
import {marginBottom} from "../Styles";

/**
 * @param {{editText: Object, sendEvent: Function}} props
 * @returns {JSX.Element}
 * @constructor
 */
export default function EditText(props) {
    const {label, valueChangeEvent, saveValueEvent} = props.editText;
    const [value, setValue] = useState(props.editText.value || "");
    const onValueChanged = e => {
        const v = e.target.value;
        setValue(v);
        props.sendEvent(Object.assign({value: v}, valueChangeEvent));
    };
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