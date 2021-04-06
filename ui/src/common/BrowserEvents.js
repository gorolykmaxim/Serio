import {useEffect} from "react";

export function callOnEnter(callback) {
    return e => {
        if (e.key === 'Enter') {
            callback();
        }
    };
}

export function useEvent(event, callback) {
    useEffect(() => {
        if (!callback) {
            return;
        }
        window.addEventListener(event, callback);
        return () => window.removeEventListener(event, callback);
    });
}