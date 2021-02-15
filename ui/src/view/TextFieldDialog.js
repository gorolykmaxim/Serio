import {Button, TextField} from "@material-ui/core";
import {useState} from "react";
import {marginBottom} from "../Styles";
import CenteredView from "../common/CenteredView";
import Text from "../common/Text";

/**
 * @param {{title: string, description: string, label: string, saveText: string, cancelText: string, value: string, onValueChange: Function, onCancel: Function, onSave: Function}} props
 * @returns {JSX.Element}
 */
function TextFieldDialog(props) {
    const margin = marginBottom();
    const [value, setValue] = useState(props.value || "");
    const onValueChanged = e => {
        const v = e.target.value;
        setValue(v);
        props.onValueChange(v);
    };
    return (
        <CenteredView>
            <Text variant="h6"
                        classes={margin}>{props.title}</Text>
            <Text color="textSecondary"
                        classes={margin}>{props.description}</Text>
            <TextField autoFocus
                       fullWidth
                       label={props.label}
                       margin="dense"
                       value={value}
                       onChange={onValueChanged}
                       classes={margin}/>
            <Button fullWidth
                    variant="contained"
                    color="primary"
                    onClick={props.onSave}
                    classes={margin}>{props.saveText}</Button>
            <Button fullWidth
                    onClick={props.onCancel}>{props.cancelText}</Button>
        </CenteredView>
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