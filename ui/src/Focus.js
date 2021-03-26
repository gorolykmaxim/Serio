import {useEffect, useRef} from "react";

export function useAutoFocusWhenReady(autoFocus) {
    const root = useRef(null);
    useEffect(() => {
        if (autoFocus) {
            root.current?.focus();
        }
    });
    return root;
}