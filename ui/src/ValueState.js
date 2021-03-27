import {useState} from "react";

export function useValue(initialValue, valueChangeEvent, sendEvent) {
    const [value, setValue] = useState(initialValue || "");
    const onValueChanged = e => {
        const v = e.target.value;
        setValue(v);
        sendEvent(Object.assign({value: v}, valueChangeEvent));
    };
    return [value, onValueChanged];
}