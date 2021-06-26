import * as bootstrap from "bootstrap";
import "typeface-passion-one";
import "./style.scss";

const TITLE_SCREEN = 0;
const EDIT_TEXT_DIALOG = 1;
const DIALOG = 2;
const LOADING_SCREEN = 3;

const titleScreenContent = {view: TITLE_SCREEN};

const editTextDialogContent = {
    view: EDIT_TEXT_DIALOG,
    dialog: {
        title: "Crawler config URL",
        description: "Specify URL to the file, that contains configuration of all the crawlers, responsible for crawling tv shows.",
        cancelText: "Cancel",
        confirmText: "Save",
        cancelEvent: {event: 1},
        confirmEvent: {event: 2},
    },
    editText: {
        label: "Crawler config URL",
        value: "https://github.com/gorolykmaxim/content.json",
        valueChangedEvent: {event: 3},
        saveValueEvent: {event: 4},
    },
};

const dialogContent = {
    view: DIALOG,
    dialog: {
        title: "Whoops...",
        description: "Something went horribly wrong :(",
        confirmText: "Understand",
        confirmEvent: {event: 4},
    }
}

const loadingScreenContent = {
    view: LOADING_SCREEN,
    loading: {
        text: "Downloading crawler config..."
    }
};

function create(tag, ...classes) {
    const e = document.createElement(tag);
    e.classList.add.apply(e.classList, classes);
    return e;
}

function createButton(core, text, event, isPrimary, marginTop) {
    const btn = create("button", "btn", isPrimary ? "btn-primary" : "btn-dark", marginTop ? "mt-2" : "");
    btn.innerText = text;
    btn.onclick = () => core.sendEvent(event);
    return btn;
}

function createEditText(core, content) {
    const {label, value, valueChangedEvent, saveValueEvent} = content;
    const editText = create("input", "form-control");
    editText.value = value || "";
    editText.setAttribute("placeholder", label);
    editText.oninput = () => core.sendEvent(Object.assign({value: editText.value}, valueChangedEvent));
    editText.onkeydown = (e) => {
        if (e.key === "Enter") {
            core.sendEvent(saveValueEvent);
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

function createDialogButton(core, text, event, isPrimary, elements, focusable) {
    if (text) {
        const btn = createButton(core, text, event, isPrimary, true);
        focusable.push(btn);
        elements.push(btn);
    }
}

function createDialog(ui, core, content, innerElements) {
    if (content.view !== DIALOG && !innerElements) return;
    const {title, description, confirmText, confirmEvent, cancelText, cancelEvent} = content.dialog;
    const elements = [];
    elements.push(createTitle(title));
    const desc = create("p", "text-muted", "not-selectable");
    desc.innerText = description;
    elements.push(desc);
    if (innerElements) {
        elements.push.apply(elements, innerElements);
    }
    const focusable = [];
    createDialogButton(core, confirmText, confirmEvent, true, elements, focusable);
    createDialogButton(core, cancelText, cancelEvent, false, elements, focusable);
    if (focusable.length > 0) {
        ui.toFocus = focusable[0];
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
    ui.displayNext.classList.add("serio-animation", "serio-animation-entering");
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
    setTimeout(() => {
        ui.currentView.classList.replace("serio-animation-entering", "serio-animation-entered")
    }, 0);
}

window.ui = {
    root: document.getElementById("root"),
    currentView: null,
    displayNext: null,
    toFocus: null,
};
window.core = {
    sendEvent: (e) => console.log(e),
};

window.displayView = function (content) {
    createTitleScreen(ui, content);
    createEditTextDialog(ui, core, content);
    createDialog(ui, core, content);
    createLoadingScreen(ui, content);
    displayElement(ui);
};

displayView(titleScreenContent);