#include "CrawlerStepEditorViewModel.h"
#include <QQmlContext>
#include <user-interface/ViewNames.h>

namespace serio::qt {

CrawlerStepEditorViewModel::CrawlerStepEditorViewModel(core::TvShowCrawlerEditor &editor,
                                                       StackOfViews &stackOfViewsController)
    : editor(editor), stackOfViewsController(stackOfViewsController) {}

void CrawlerStepEditorViewModel::initialize(ActionRouter &router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("crawlerStepEditorViewModel", this);
    router.registerAction(ActionType::OPEN_NEW_CRAWLER_STEP_EDITOR, [this] (const auto& args) { openNew(); });
    router.registerAction(ActionType::OPEN_EXISTING_CRAWLER_STEP_EDITOR, [this] (const auto& args) { openExisting(args); });
    router.registerAction(ActionType::LOAD_CRAWLER_STEP, [this] (const auto& args) { load(); });
    router.registerAction(ActionType::SELECT_CRAWLER_STEP_TYPE, [this] (const auto& args) { selectType(args); });
    router.registerAction(ActionType::SAVE_CRAWLER_STEP, [this] (const auto& args) { save(); });
    router.registerAction(ActionType::REMOVE_CRAWLER_STEP, [this] (const auto& args) { remove(); });
}

bool CrawlerStepEditorViewModel::isExistingStep() const {
    return editedStepIndex.has_value();
}

QList<RadioButtonModel*> CrawlerStepEditorViewModel::getCrawlerStepTypes() const {
    return typeOptions;
}

QString CrawlerStepEditorViewModel::getDescription() const {
    return description;
}

QList<TextFieldModel*> CrawlerStepEditorViewModel::getProperties() const {
    return properties;
}

void CrawlerStepEditorViewModel::openNew() {
    modifyModel([this] {
        editedStepIndex.reset();
        emit isExistingStepChanged();
        populateActions(false);
        selectedType.reset();
        stackOfViewsController.pushView(crawlerStepEditorView);
    });
}

void CrawlerStepEditorViewModel::openExisting(const QVariantList& args) {
    auto stepIndex = args[0].toUInt();
    modifyModel([this, stepIndex] {
        editedStepIndex = stepIndex;
        emit isExistingStepChanged();
        populateActions(true);
        selectedType.reset();
        stackOfViewsController.pushView(crawlerStepEditorView);
    });
}

void CrawlerStepEditorViewModel::load() {
    types = editor.getCrawlerStepTypes();
    std::optional<core::CrawlerStep> editedStep;
    if (editedStepIndex) {
        editedStep = editor.getCrawlerSteps()[*editedStepIndex];
    }
    modifyModel([this, editedStep] {
        initializeCrawlerStepTypes();
        if (editedStep) {
            setSelectedType([&editedStep] (const auto& type) {return type.getName() == editedStep->getType();});
            setProperties(*editedStep);
        } else {
            setProperties(*selectedType);
        }
    });
}

void CrawlerStepEditorViewModel::selectType(const QVariantList &args) {
    auto name = args[0].toString().toStdString();
    modifyModel([this, name] {
        setSelectedType([&name] (const auto &type) { return type.getName() == name; });
        setProperties(*selectedType);
    });
}

void CrawlerStepEditorViewModel::save() {
    auto step = createCrawlerStep();
    if (editedStepIndex) {
        editor.replaceCrawlerStep(*editedStepIndex, step);
    } else {
        editor.addCrawlerStep(step);
    }
    stackOfViewsController.popCurrentView();
}

void CrawlerStepEditorViewModel::remove() {
    editor.removeCrawlerStep(*editedStepIndex);
    stackOfViewsController.popCurrentView();
}

void CrawlerStepEditorViewModel::initializeCrawlerStepTypes() {
    typeOptions.clearAndDelete();
    emit crawlerStepTypesChanged();
    for (const auto& type: types) {
        if (!selectedType) {
            setSelectedType(type);
        }
        typeOptions << new RadioButtonModel(QString::fromStdString(type.getName()), *selectedType == type);
    }
    emit crawlerStepTypesChanged();
}

void CrawlerStepEditorViewModel::setProperties(const core::CrawlerStepType &type) {
    properties.clearAndDelete();
    emit propertiesChanged();
    for (const auto& propertyName: type.getMandatoryProperties()) {
        properties << new TextFieldModel(QString::fromStdString(propertyName));
    }
    emit propertiesChanged();
}

void CrawlerStepEditorViewModel::setProperties(const core::CrawlerStep &step) {
    properties.clearAndDelete();
    emit propertiesChanged();
    for (const auto& property: step.getProperties()) {
        auto name = QString::fromStdString(property.first);
        auto value = QString::fromStdString(property.second);
        properties.append(new TextFieldModel(name, value));
    }
    emit propertiesChanged();
}

void CrawlerStepEditorViewModel::setSelectedType(const core::CrawlerStepType &type) {
    selectedType = type;
    description = QString::fromStdString(type.getDescription());
    emit descriptionChanged();
    auto typeName = QString::fromStdString(selectedType->getName());
    for (const auto& stepType: typeOptions) {
        stepType->setChecked(stepType->getName() == typeName);
    }
}

void CrawlerStepEditorViewModel::setSelectedType(const std::function<bool(const core::CrawlerStepType &)>& predicate) {
    auto typeIt = std::find_if(types.cbegin(), types.cend(), predicate);
    if (typeIt != types.cend()) {
        setSelectedType(*typeIt);
    }
}

core::CrawlerStep CrawlerStepEditorViewModel::createCrawlerStep() const {
    std::map<std::string, std::string> props;
    for (const auto& property: properties) {
        props[property->getName().toStdString()] = property->getValue().toStdString();
    }
    return core::CrawlerStep(selectedType->getName(), props);
}

QList<ButtonModel*> CrawlerStepEditorViewModel::getActions() const {
    return actions;
}

void CrawlerStepEditorViewModel::populateActions(bool isDeleteEnabled) {
    actions.clearAndDelete();
    emit actionsChanged();
    actions << new ButtonModel("cancel", ActionType::BACK, {}, false);
    actions << new ButtonModel("save", ActionType::SAVE_CRAWLER_STEP);
    if (isDeleteEnabled) {
        actions << new ButtonModel("delete", ActionType::REMOVE_CRAWLER_STEP);
    }
    emit actionsChanged();
}

}