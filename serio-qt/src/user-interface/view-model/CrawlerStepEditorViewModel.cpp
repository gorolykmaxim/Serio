#include "CrawlerStepEditorViewModel.h"
#include <QQmlContext>
#include <user-interface/ViewNames.h>

serio::qt::CrawlerStepEditorViewModel::CrawlerStepEditorViewModel(core::TvShowCrawlerEditor &editor,
                                                                  StackOfViews &stackOfViewsController)
    : editor(editor), stackOfViewsController(stackOfViewsController) {}

void serio::qt::CrawlerStepEditorViewModel::initialize(serio::qt::ActionRouter &router, QQmlApplicationEngine &engine) {
    engine.rootContext()->setContextProperty("crawlerStepEditorViewModel", this);
    router.registerAction(ActionType::OPEN_NEW_CRAWLER_STEP_EDITOR, [this] (const QVariantList& args) { openNew(); });
    router.registerAction(ActionType::OPEN_EXISTING_CRAWLER_STEP_EDITOR, [this] (const QVariantList& args) { openExisting(args); });
    router.registerAction(ActionType::LOAD_CRAWLER_STEP, [this] (const QVariantList& args) { load(); });
    router.registerAction(ActionType::SELECT_CRAWLER_STEP_TYPE, [this] (const QVariantList& args) { selectType(args); });
    router.registerAction(ActionType::SAVE_CRAWLER_STEP, [this] (const QVariantList& args) { save(); });
    router.registerAction(ActionType::REMOVE_CRAWLER_STEP, [this] (const QVariantList& args) { remove(); });
}

bool serio::qt::CrawlerStepEditorViewModel::isExistingStep() const {
    return editedStepIndex.has_value();
}

QList<serio::qt::RadioButtonModel*> serio::qt::CrawlerStepEditorViewModel::getCrawlerStepTypes() {
    return typeOptions;
}

QString serio::qt::CrawlerStepEditorViewModel::getDescription() const {
    return description;
}

QList<serio::qt::TextFieldModel*> serio::qt::CrawlerStepEditorViewModel::getProperties() const {
    return properties;
}

void serio::qt::CrawlerStepEditorViewModel::openNew() {
    modifyModel([this] {
        editedStepIndex.reset();
        emit isExistingStepChanged();
        selectedType.reset();
        stackOfViewsController.pushView(crawlerStepEditorView);
    });
}

void serio::qt::CrawlerStepEditorViewModel::openExisting(const QVariantList& args) {
    unsigned int stepIndex = args[0].toUInt();
    modifyModel([this, stepIndex] {
        editedStepIndex = stepIndex;
        emit isExistingStepChanged();
        selectedType.reset();
        stackOfViewsController.pushView(crawlerStepEditorView);
    });
}

void serio::qt::CrawlerStepEditorViewModel::load() {
    types = editor.getCrawlerStepTypes();
    std::optional<core::CrawlerStep> editedStep;
    if (editedStepIndex) {
        editedStep = editor.getCrawlerSteps()[*editedStepIndex];
    }
    modifyModel([this, editedStep] {
        initializeCrawlerStepTypes();
        if (editedStep) {
            setSelectedType([&editedStep](const core::CrawlerStepType &type) {
                return type.getName() == editedStep->getType();
            });
            setProperties(*editedStep);
        } else {
            setProperties(*selectedType);
        }
    });
}

void serio::qt::CrawlerStepEditorViewModel::selectType(const QVariantList &args) {
    std::string name = args[0].toString().toStdString();
    modifyModel([this, name] {
        setSelectedType([&name](const core::CrawlerStepType &type) { return type.getName() == name; });
        setProperties(*selectedType);
    });
}

void serio::qt::CrawlerStepEditorViewModel::save() {
    core::CrawlerStep step = createCrawlerStep();
    if (editedStepIndex) {
        editor.replaceCrawlerStep(*editedStepIndex, step);
    } else {
        editor.addCrawlerStep(step);
    }
    stackOfViewsController.popCurrentView();
}

void serio::qt::CrawlerStepEditorViewModel::remove() {
    editor.removeCrawlerStep(*editedStepIndex);
    stackOfViewsController.popCurrentView();
}

void serio::qt::CrawlerStepEditorViewModel::initializeCrawlerStepTypes() {
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

void serio::qt::CrawlerStepEditorViewModel::setProperties(const serio::core::CrawlerStepType &type) {
    properties.clearAndDelete();
    emit propertiesChanged();
    for (const auto& propertyName: type.getMandatoryProperties()) {
        properties << new TextFieldModel(QString::fromStdString(propertyName));
    }
    emit propertiesChanged();
}

void serio::qt::CrawlerStepEditorViewModel::setProperties(const serio::core::CrawlerStep &step) {
    properties.clearAndDelete();
    emit propertiesChanged();
    for (const auto& property: step.getProperties()) {
        QString name = QString::fromStdString(property.first);
        QString value = QString::fromStdString(property.second);
        properties.append(new TextFieldModel(name, value));
    }
    emit propertiesChanged();
}

void serio::qt::CrawlerStepEditorViewModel::setSelectedType(const serio::core::CrawlerStepType &type) {
    selectedType = type;
    description = QString::fromStdString(type.getDescription());
    emit descriptionChanged();
    QString typeName = QString::fromStdString(selectedType->getName());
    for (const auto& stepType: typeOptions) {
        stepType->setChecked(stepType->getName() == typeName);
    }
}

void serio::qt::CrawlerStepEditorViewModel::setSelectedType(const std::function<bool(const core::CrawlerStepType &)>& predicate) {
    auto typeIt = std::find_if(types.cbegin(), types.cend(), predicate);
    if (typeIt != types.cend()) {
        setSelectedType(*typeIt);
    }
}

serio::core::CrawlerStep serio::qt::CrawlerStepEditorViewModel::createCrawlerStep() const {
    std::map<std::string, std::string> props;
    for (const auto& property: properties) {
        props[property->getName().toStdString()] = property->getValue().toStdString();
    }
    return core::CrawlerStep(selectedType->getName(), props);
}
