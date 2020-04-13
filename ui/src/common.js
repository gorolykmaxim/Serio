export function createScrollIntoTarget(isHorizontalScroll) {
    if (isHorizontalScroll) {
        return e => e.target.parentElement.scroll({left: e.target.offsetLeft - window.innerWidth / 2 + e.target.offsetWidth / 2});
    } else {
        return e => e.target.parentElement.scroll({top: e.target.offsetTop - window.innerHeight / 2 + e.target.offsetHeight / 2});
    }
}

const noopFunction = () => {};

export function getFunction(f) {
    return f || noopFunction;
}
