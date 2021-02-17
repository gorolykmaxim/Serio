export function callOnEnter(callback) {
    return e => {
        if (e.key === 'Enter') {
            callback();
        }
    };
}