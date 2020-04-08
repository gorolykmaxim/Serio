export function scrollIntoTarget(event) {
    const target = event.target;
    target.parentElement.scroll({
        left: target.offsetLeft - window.innerWidth / 2 + target.offsetWidth / 2
    });
}


export function getFunction(f) {
    return f || (() => {});
}
