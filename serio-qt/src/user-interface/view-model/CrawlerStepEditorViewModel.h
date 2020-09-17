#ifndef SERIO_CRAWLERSTEPEDITORVIEWMODEL_H
#define SERIO_CRAWLERSTEPEDITORVIEWMODEL_H

#include <tv-show-crawler-editor/TvShowCrawlerEditor.h>
#include <user-interface/model/RadioButtonModel.h>
#include <user-interface/model/TextFieldModel.h>
#include <user-interface/model/ListModel.h>
#include <user-interface/action/ActionRouter.h>
#include "ViewModel.h"

namespace serio::qt {

class CrawlerStepEditorViewModel : public ViewModel {
    Q_OBJECT
    Q_PROPERTY(bool isExistingStep READ isExistingStep NOTIFY isExistingStepChanged)
    Q_PROPERTY(QList<RadioButtonModel*> crawlerStepTypes READ getCrawlerStepTypes NOTIFY crawlerStepTypesChanged)
    Q_PROPERTY(QString description READ getDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QList<TextFieldModel*> properties READ getProperties NOTIFY propertiesChanged)
public:
    CrawlerStepEditorViewModel(core::TvShowCrawlerEditor &editor, StackOfViews &stackOfViewsController);
    void initialize(ActionRouter& router, QQmlApplicationEngine& engine);
    [[nodiscard]] bool isExistingStep() const;
    [[nodiscard]] QList<RadioButtonModel*> getCrawlerStepTypes();
    [[nodiscard]] QString getDescription() const;
    [[nodiscard]] QList<TextFieldModel*> getProperties() const;
    void openNew();
    void openExisting(const QVariantList& args);
    void load();
    void selectType(const QVariantList& args);
    void save();
    void remove();
signals:
    void isExistingStepChanged();
    void crawlerStepTypesChanged();
    void descriptionChanged();
    void propertiesChanged();
private:
    const QString viewName = "CrawlerStepEditorView.qml";
    std::optional<unsigned int> editedStepIndex;
    std::optional<core::CrawlerStepType> selectedType;
    QString description;
    std::vector<core::CrawlerStepType> types;
    ListModel<RadioButtonModel*> typeOptions;
    ListModel<TextFieldModel*> properties;
    core::TvShowCrawlerEditor& editor;
    StackOfViews& stackOfViewsController;
    void initializeCrawlerStepTypes();
    void setProperties(const core::CrawlerStepType& type);
    void setProperties(const core::CrawlerStep& step);
    void setSelectedType(const core::CrawlerStepType& type);
    void setSelectedType(const std::function<bool(const core::CrawlerStepType&)>& predicate);
    [[nodiscard]] core::CrawlerStep createCrawlerStep() const;
};

}

#endif //SERIO_CRAWLERSTEPEDITORVIEWMODEL_H
