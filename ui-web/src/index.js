import * as bootstrap from "bootstrap";
import "typeface-passion-one";
import "./style.scss";

const dummyContent = {
    dialog: {
        title: "Crawler config URL",
        description: "Specify URL to the file, that contains configuration of all the crawlers, responsible for crawling tv shows.",
        cancelText: "Cancel",
        confirmText: "Save"
    }
};

function create(tag, ...classes) {
    const e = document.createElement(tag);
    e.classList.add.apply(e.classList, classes);
    return e;
}

function createTitleScreen(ui) {
    const root = create("div", "center-layout", "full-height");
    const title = create("h1", "serio-title", "not-selectable", "text-primary");
    title.innerText = "Serio";
    root.appendChild(title);
    ui.displayNext = root;
}

function createEditTextDialog(ui, content) {
    const root = create("div", "center-layout", "full-height");
    const container = create("div", "center-content", "full-height", "smartphone-max-width");
    const title = create("h3", "center-text", "not-selectable");
    title.innerText = content.dialog.title;
    container.appendChild(title);
    const description = create("p", "text-muted", "not-selectable");
    description.innerText = content.dialog.description;
    container.appendChild(description);
    const textField = create("input", "form-control");
    textField.setAttribute("placeholder", "Crawler config URL");
    container.appendChild(textField);
    const save = create("button", "btn", "btn-primary", "mt-2");
    save.innerText = content.dialog.confirmText;
    container.appendChild(save);
    const cancel = create("button", "btn", "btn-dark", "mt-2");
    cancel.innerText = content.dialog.cancelText;
    container.appendChild(cancel);
    root.appendChild(container);
    ui.displayNext = root;
    ui.toFocus = textField;
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
createTitleScreen(ui);
createEditTextDialog(ui, dummyContent);
displayElement(ui);