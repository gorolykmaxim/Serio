import * as bootstrap from "bootstrap";
import "typeface-passion-one";
import "./style.scss";

const dummyContent = {
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

const dummyErrorContent = {
    dialog: {
        title: "Whoops...",
        description: "Something went horribly wrong :(",
        confirmText: "Understand",
        confirmEvent: {event: 4},
    }
}

const dummyLoadingContent = {
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

function createTitleScreen(ui) {
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
    const {confirmText, confirmEvent, cancelText, cancelEvent} = content.dialog;
    const elements = [];
    elements.push(createTitle(content.dialog.title));
    const description = create("p", "text-muted", "not-selectable");
    description.innerText = content.dialog.description;
    elements.push(description);
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
    const editText = createEditText(core, content.editText);
    createDialog(ui, core, content, [editText]);
    ui.toFocus = editText;
}

function createLoadingScreen(ui, content) {
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

const ui = {
    root: document.getElementById("root"),
    currentView: null,
    displayNext: null,
    toFocus: null,
};
const core = {
    sendEvent: (e) => console.log(e),
};
createTitleScreen(ui);
createEditTextDialog(ui, core, dummyContent);
createDialog(ui, core, dummyErrorContent);
createLoadingScreen(ui, dummyLoadingContent);
displayElement(ui);