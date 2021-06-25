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

function create(tag, ...classes) {
    const e = document.createElement(tag);
    e.classList.add.apply(e.classList, classes);
    return e;
}

function createButton(ui, text, event, isPrimary, marginTop) {
    const btn = create("button", "btn", isPrimary ? "btn-primary" : "btn-dark", marginTop ? "mt-2" : "");
    btn.innerText = text;
    btn.onclick = () => ui.core.sendEvent(event);
    return btn;
}

function createEditText(ui, content) {
    const {label, value, valueChangedEvent, saveValueEvent} = content;
    const editText = create("input", "form-control");
    editText.value = value || "";
    editText.setAttribute("placeholder", label);
    editText.oninput = () => ui.core.sendEvent(Object.assign({value: editText.value}, valueChangedEvent));
    editText.onkeydown = (e) => {
        if (e.key === "Enter") {
            ui.core.sendEvent(saveValueEvent);
        }
    };
    return editText;
}

function createTitleScreen(ui) {
    const root = create("div", "center-layout", "full-height");
    const title = create("h1", "serio-title", "not-selectable", "text-primary");
    title.innerText = "Serio";
    root.appendChild(title);
    ui.displayNext = root;
}

function createEditTextDialog(ui, content) {
    const {confirmText, confirmEvent, cancelText, cancelEvent} = content.dialog;
    const root = create("div", "center-layout", "full-height");
    const container = create("div", "center-content", "full-height", "smartphone-max-width", "p-4");
    const title = create("h3", "center-text", "not-selectable");
    title.innerText = content.dialog.title;
    container.appendChild(title);
    const description = create("p", "text-muted", "not-selectable");
    description.innerText = content.dialog.description;
    container.appendChild(description);
    const editText = createEditText(ui, content.editText);
    container.appendChild(editText);
    container.appendChild(createButton(ui, confirmText, confirmEvent, true, true));
    container.appendChild(createButton(ui, cancelText, cancelEvent, false, true));
    root.appendChild(container);
    ui.displayNext = root;
    ui.toFocus = editText;
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
    core: {
        sendEvent: (e) => console.log(e),
    },
    currentView: null,
    displayNext: null,
    toFocus: null,
};
createTitleScreen(ui);
createEditTextDialog(ui, dummyContent);
displayElement(ui);