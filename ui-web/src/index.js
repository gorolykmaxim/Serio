import * as bootstrap from "bootstrap";
import "typeface-passion-one";
import "./style.scss";

const TITLE_SCREEN = 0;
const EDIT_TEXT_DIALOG = 1;
const DIALOG = 2;
const LOADING_SCREEN = 3;

const FAST_ANIMATION = 0;
const SLOW_ANIMATION = 1;

function sendEvent(core, event) {
    core.sendEvent(JSON.stringify(event));
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

function createButton(core, text, event, isPrimary, marginTop) {
    const btn = create("button", "btn", isPrimary ? "btn-primary" : "btn-dark", marginTop ? "mt-2" : "");
    btn.innerText = text;
    btn.onclick = () => sendEvent(core, event);
    return btn;
}

function createEditText(core, content) {
    const {label, value, valueChangedEvent, saveValueEvent} = content;
    const editText = create("input", "form-control");
    editText.value = value || "";
    editText.setAttribute("placeholder", label);
    editText.oninput = () => sendEvent(core, Object.assign({value: editText.value}, valueChangedEvent));
    editText.onkeydown = (e) => {
        if (e.key === "Enter") {
            sendEvent(core, saveValueEvent);
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

function createTitleScreen(ui, content) {
    if (content.view !== TITLE_SCREEN) return;
    const root = create("div", "center-layout", "full-height");
    const title = create("h1", "serio-title", "not-selectable", "text-primary");
    title.innerText = "Serio";
    root.appendChild(title);
    ui.displayNext = root;
}

function createDialogButton(core, text, event, buttons) {
    if (event) {
        const btn = createButton(core, text, event, buttons.length === 0, true);
        buttons.push(btn);
    }
}

function createDialog(ui, core, content, innerElements) {
    if (content.view !== DIALOG && !innerElements) return;
    const {title, description, confirmText, confirmEvent, cancelText} = content.dialog;
    const elements = [];
    elements.push(createTitle(title));
    const desc = create("p", "text-muted", "not-selectable");
    desc.innerText = description;
    elements.push(desc);
    if (innerElements) {
        elements.push.apply(elements, innerElements);
    }
    const buttons = [];
    createDialogButton(core, confirmText, confirmEvent, buttons);
    createDialogButton(core, cancelText, content.backEvent, buttons);
    elements.push.apply(elements, buttons);
    if (buttons.length > 0) {
        ui.toFocus = buttons[0];
    }
    ui.displayNext = createCenteredLayout(elements);
}

function createEditTextDialog(ui, core, content) {
    if (content.view !== EDIT_TEXT_DIALOG) return;
    const editText = createEditText(core, content.editText);
    createDialog(ui, core, content, [editText]);
    ui.toFocus = editText;
}

function createLoadingScreen(ui, content) {
    if (content.view !== LOADING_SCREEN) return;
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
    ui.displayNext = createCenteredLayout(elements);
}

function displayElement(ui) {
    if (!ui.displayNext) return;
    if (ui.currentView) {
        ui.root.replaceChild(ui.displayNext, ui.currentView);
    } else {
        ui.root.appendChild(ui.displayNext);
    }
    if (ui.toFocus) {
        ui.toFocus.focus();
    }
    ui.currentView = ui.displayNext;
    ui.displayNext = null;
}

function animationToCSSClasses(animation) {
    const classes = [];
    if (animation.fade !== false) {
        classes.push("a-fade");
    }
    if (animation.scale !== false) {
        classes.push("a-scale");
    }
    return classes;
}

function animateElement(ui, content, animationState) {
    if (ui.displayNext) {
        // New view is requested to be rendered.
        const animation = animationState.currentViewAnimation || {};
        const classesToApply = animationToCSSClasses(animation);
        if (classesToApply.length > 0 && ui.currentView) {
            // Don't trigger transition-out animation if it is already running, just replace the view, that needs
            // be displayed after it.
            if (!animationState.viewToDisplayAfterAnimation) {
                classesToApply.push("a-reverse");
                classesToApply.forEach(c => ui.currentView.classList.add(c));
                ui.currentView.onanimationend = renderUI;
            }
            animationState.viewToDisplayAfterAnimation = removeProperty(ui, "displayNext");
            animationState.nextViewAnimation = content.animation || {};
        } else {
            animationState.currentViewAnimation = content.animation || {};
        }
    } else if (animationState.viewToDisplayAfterAnimation) {
        // Transition-out animation for current view has finished.
        animationState.currentViewAnimation = removeProperty(animationState, "nextViewAnimation");
        ui.displayNext = removeProperty(animationState, "viewToDisplayAfterAnimation");
    }
    if (ui.displayNext) {
        const animation = animationState.currentViewAnimation;
        const classList = ui.displayNext.classList;
        classList.add(animation.speed === SLOW_ANIMATION ? "a-slow" : "a-fast");
        const classesToRemoveLater = animationToCSSClasses(animation);
        classesToRemoveLater.forEach(a => classList.add(a));
        ui.displayNext.onanimationend = () => classList.remove.apply(classList, classesToRemoveLater);
    }
}

function renderUI() {
    const content = removeProperty(window, "content") || {};
    createTitleScreen(ui, content);
    createEditTextDialog(ui, core, content);
    createDialog(ui, core, content);
    createLoadingScreen(ui, content);
    animateElement(ui, content, animationState);
    displayElement(ui);
}

window.ui = {
    root: document.getElementById("root"),
    currentView: null,
    displayNext: null,
    toFocus: null,
};
window.animationState = {
    currentViewAnimation: null,
    nextViewAnimation: null,
    viewToDisplayAfterAnimation: null,
};
if (!window.core) {
    // In case we are not running inside WebView but in standalone browser - create dummy core to make UI not fail.
    window.core = {
        sendEvent: console.log,
    };
}
window.displayView = function (content) {
    window.content = content;
    renderUI();
};