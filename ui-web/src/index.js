import * as bootstrap from "bootstrap";
import "typeface-passion-one";
import "./style.scss";

function create(tag, ...classes) {
    const e = document.createElement(tag);
    e.classList.add.apply(e.classList, classes);
    return e;
}

function createTitleScreen(ui) {
    const root = create("div", "serio-title", "full-height");
    const title = create("h1", "display-1", "not-selectable");
    title.innerText = "Serio";
    root.appendChild(title);
    ui.displayNext = root;
}

function displayElement(ui) {
    if (!ui.displayNext) return;
    ui.displayNext.classList.add("serio-animation", "serio-animation-entering");
    if (ui.currentView) {
        ui.root.replaceChild(ui.displayNext, ui.currentView);
    } else {
        ui.root.appendChild(ui.displayNext);
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
    displayNext: null
};
createTitleScreen(ui);
displayElement(ui);