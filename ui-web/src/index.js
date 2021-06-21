import "./style.scss";
import "typeface-passion-one";

function createTitleScreen(ui) {
    const e = document.createElement("div");
    e.classList.add("serio-title", "full-height", "not-selectable", "mdc-theme--primary", "mdc-typography--headline1");
    e.innerText = "Serio";
    ui.displayNext = e;
}

function displayElement(ui) {
    if (!ui.displayNext) return;
    if (ui.currentView) {
        ui.root.replaceChild(ui.displayNext, ui.currentView);
    } else {
        ui.root.appendChild(ui.displayNext);
    }
    ui.currentView = ui.displayNext;
    ui.displayNext = null;
}

const ui = {
    root: document.getElementById("root"),
    currentView: null,
    displayNext: null
};
createTitleScreen(ui);
displayElement(ui);