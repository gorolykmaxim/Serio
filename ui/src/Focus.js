import {useEffect, useRef} from "react";

let requestedFocusTarget = null;

export function startContentAnimation(duration) {
    setTimeout(() => {
        requestedFocusTarget?.focus();
        requestedFocusTarget = null;
    }, duration);
}

export function useAutoFocusWhenReady(autoFocus) {
    const root = useRef(null);
    useEffect(() => {
        if (autoFocus) {
            focusWhenReady(root.current);
        }
    });
    return root;
}

export function focusWhenReady(target) {
    requestedFocusTarget = target;
}