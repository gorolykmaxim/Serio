import * as bootstrap from "bootstrap";
import "typeface-passion-one";
import "./style.scss";

const TITLE_SCREEN = 0;
const EDIT_TEXT_DIALOG = 1;
const DIALOG = 2;
const LOADING_SCREEN = 3;

const FAST_ANIMATION = 0;
const SLOW_ANIMATION = 1;

const BODY_ID = 0;

function findById(arr, id) {
    const result = arr.filter(a => a.id === id);
    return result.length > 0 ? result[0] : null;
}

function moveFromTo(id, arr1, arr2) {
    for (let i = 0; i < arr1.length; i++) {
        const e = arr1[i];
        if (e.id === id) {
            arr1.splice(i, 1);
            arr2.push(e);
            i--;
        }
    }
}

function clear(arr) {
    arr.splice(0, arr.length);
}

function sendTask(core, task) {
    core.sendTask(JSON.stringify(task));
}

function removeProperty(obj, name) {
    const value = obj[name];
    obj[name] = null;
    return value;
}

function create(tag, ...classes) {
    const e = document.createElement(tag);
    e.classList.add.apply(e.classList, classes);
    return e;
}

function createButton(core, text, task, isPrimary, marginTop) {
    const btn = create("button", "btn", isPrimary ? "btn-primary" : "btn-dark", marginTop ? "mt-2" : "");
    btn.innerText = text;
    btn.onclick = () => sendTask(core, task);
    return btn;
}

function createEditText(core, content) {
    const {label, value, valueChangedTask, saveValueTask} = content;
    const editText = create("input", "form-control");
    editText.value = value || "";
    editText.setAttribute("placeholder", label);
    editText.oninput = () => {
        const event = Object.assign({}, valueChangedTask);
        event.args = [editText.value].concat(valueChangedTask.args);
        sendTask(core, event);
    };
    editText.onkeydown = (e) => {
        if (e.key === "Enter") {
            sendTask(core, saveValueTask);
        }
    };
    return editText;
}

function createTitle(text) {
    const title = create("h3", "text-center", "not-selectable");
    title.innerText = text;
    return title;
}

function createCenteredLayout(elements) {
    const root = create("div", "center-layout", "full-height");
    const container = create("div", "center-content", "full-height", "smartphone-max-width", "p-4");
    if (elements) {
        elements.forEach(e => container.appendChild(e));
    }
    root.appendChild(container);
    return root;
}

function createTitleScreen(viewsToDisplay, content) {
    if (content.viewId !== TITLE_SCREEN) return;
    const root = create("div", "center-layout", "full-height");
    const title = create("h1", "serio-title", "not-selectable", "text-primary");
    title.innerText = "Serio";
    root.appendChild(title);
    viewsToDisplay.push({id: BODY_ID, element: root});
}

function createDialogButton(core, text, task, buttons) {
    if (task) {
        const btn = createButton(core, text, task, buttons.length === 0, true);
        buttons.push(btn);
    }
}

function createDialog(viewsToDisplay, core, content, innerElements) {
    if (content.viewId !== DIALOG && !innerElements) return;
    const {title, description, confirmText, confirmTask, cancelText} = content.dialog;
    const elements = [];
    elements.push(createTitle(title));
    const desc = create("p", "text-muted", "not-selectable", "word-break");
    desc.innerText = description;
    elements.push(desc);
    if (innerElements) {
        elements.push.apply(elements, innerElements);
    }
    const buttons = [];
    createDialogButton(core, confirmText, confirmTask, buttons);
    createDialogButton(core, cancelText, content.backTask, buttons);
    elements.push.apply(elements, buttons);
    const view = {id: BODY_ID, element: createCenteredLayout(elements)};
    if (buttons.length > 0) {
        view.toFocus = buttons[0];
    }
    viewsToDisplay.push(view);
}

function createEditTextDialog(viewsToDisplay, core, content) {
    if (content.viewId !== EDIT_TEXT_DIALOG) return;
    const editText = createEditText(core, content.editText);
    const dialogView = [];
    createDialog(dialogView, core, content, [editText]);
    const dialog = dialogView[0];
    dialog.toFocus = editText;
    viewsToDisplay.push(dialog);
}

function createLoadingScreen(viewsToDisplay, content) {
    if (content.viewId !== LOADING_SCREEN) return;
    const {text} = content.loading;
    const elements = [];
    const spinnerContainer = create("div", "text-center");
    const spinner = create("div", "spinner-border", "text-primary", "mb-4");
    spinner.style.width = "8rem";
    spinner.style.height = "8rem";
    spinner.style.borderWidth = "0.75rem";
    const loading = create("span", "visually-hidden");
    loading.innerText = "Loading...";
    spinner.appendChild(loading);
    spinnerContainer.appendChild(spinner);
    elements.push(spinnerContainer);
    elements.push(createTitle(text));
    viewsToDisplay.push({id: BODY_ID, element: createCenteredLayout(elements)});
}

function displayElement(viewsToDisplay, containers) {
    for (const view of viewsToDisplay) {
        const container = containers[view.id];
        if (container.children.length) {
            container.replaceChild(view.element, container.children[0]);
        } else {
            container.appendChild(view.element);
        }
        if (view.toFocus) {
            view.toFocus.focus();
        }
    }
    clear(viewsToDisplay);
}

function animationToKeyframesName(animation) {
    const keyframes = [];
    if (animation.fade !== false) {
        keyframes.push("fade");
    }
    if (animation.scale !== false) {
        keyframes.push("scale");
    }
    return keyframes.join(", ");
}

function animate(viewsToDisplay, runningAnimations, finishedAnimations, animations, containers, content) {
    // Animate existing views out.
    for (const view of viewsToDisplay) {
        const running = findById(runningAnimations, view.id);
        if (running) {
            running.viewToDisplay = view;
        } else {
            const animation = animations[view.id];
            const keyframes = animationToKeyframesName(animation);
            const container = containers[view.id];
            if (keyframes && container.children.length > 0) {
                const previousView = container.children[0];
                previousView.classList.add("a-reverse");
                previousView.style.animationName = keyframes;
                previousView.onanimationend = () => {
                    moveFromTo(view.id, runningAnimations, finishedAnimations);
                    renderUI();
                };
                runningAnimations.push({id: view.id, viewToDisplay: view});
            } else {
                finishedAnimations.push({id: view.id, viewToDisplay: view});
            }
        }
        const {id, ...animation} = findById(content.animations || [], view.id) || {};
        animations[view.id] = animation;
    }
    clear(viewsToDisplay);
    // Animate new views in.
    for (const finishedAnimation of finishedAnimations) {
        const animation = animations[finishedAnimation.id];
        const element = finishedAnimation.viewToDisplay.element;
        element.classList.add(animation.speed === SLOW_ANIMATION ? "a-slow" : "a-fast");
        element.style.animationName = animationToKeyframesName(animation);
        element.onanimationend = () => element.style.animationName = "none";
        viewsToDisplay.push(finishedAnimation.viewToDisplay);
    }
    clear(finishedAnimations);
}

function renderUI() {
    const content = removeProperty(window, "content") || {};
    const viewsToDisplay = [];
    createTitleScreen(viewsToDisplay, content);
    createEditTextDialog(viewsToDisplay, core, content);
    createDialog(viewsToDisplay, core, content);
    createLoadingScreen(viewsToDisplay, content);
    animate(viewsToDisplay, ui.runningAnimations, ui.finishedAnimations, ui.animations, ui.containers, content);
    displayElement(viewsToDisplay, ui.containers);
}

window.ui = {
    containers: [
        document.getElementById("body")
    ],
    animations: [{}],
    runningAnimations: [],
    finishedAnimations: [],
};
if (!window.core) {
    // In case we are not running inside WebView but in standalone browser - create dummy core to make UI not fail.
    window.core = {
        sendTask: console.log,
    };
}
window.displayView = function (content) {
    window.content = content;
    renderUI();
};

// Keeps this for testing it is too useful to delete
// const titleScreenContent = {
//     viewId: TITLE_SCREEN,
//     animations: [{id: BODY_ID, speed: SLOW_ANIMATION, scale: false}],
// };
//
// const editTextDialogContent = {
//     viewId: EDIT_TEXT_DIALOG,
//     dialog: {
//         title: "Crawler config URL",
//         description: "Specify URL to the file, that contains configuration of all the crawlers, responsible for crawling tv shows.",
//         cancelText: "Cancel",
//         confirmText: "Save",
//         confirmTask: {id: 2},
//     },
//     editText: {
//         label: "Crawler config URL",
//         value: "https://github.com/gorolykmaxim/content.json",
//         valueChangedTask: {id: 3},
//         saveValueTask: {id: 4},
//     },
//     backTask: {id: 1},
//     animations: [{id: BODY_ID, scale: false, fade: false}],
// };
//
// const dialogContent = {
//     viewId: DIALOG,
//     dialog: {
//         title: "Whoops...",
//         description: "Something went horribly wrong :(",
//         cancelText: "Understand",
//     },
//     backTask: {id: 1},
// }
//
// const loadingScreenContent = {
//     viewId: LOADING_SCREEN,
//     loading: {
//         text: "Downloading crawler config..."
//     }
// };
//
// displayView(dialogContent);
// setTimeout(() => displayView(dialogContent), 3000);
// setTimeout(() => displayView(editTextDialogContent), 3100);
// setTimeout(() => displayView(loadingScreenContent), 6000);
// setTimeout(() => displayView(titleScreenContent), 9000);
