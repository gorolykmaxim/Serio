#include <gtest/gtest.h>
#include <TvShowCrawlerEditorMock.h>
#include <user-interface/StackOfViews.h>
#include <user-interface/view-model/CrawlerStepEditorViewModel.h>
#include <QSignalSpy>
#include <StackOfViewsMock.h>
#include <user-interface/ViewNames.h>
#include <user-interface/model/ButtonModel.h>

class CrawlerStepEditorViewModelTest : public ::testing::Test {
protected:
    const std::vector<serio::core::CrawlerStepType> types = {
        serio::core::CrawlerStepType("type 1", "description 1", {"property 1", "property 2"}),
        serio::core::CrawlerStepType("type 2", "description 2"),
    };
    const std::vector<serio::core::CrawlerStep> steps {
        serio::core::CrawlerStep("type 2"),
        serio::core::CrawlerStep("type 1", {{"property 1", "value 1"}, {"property 2", "value 2"}})
    };
    ::testing::NiceMock<TvShowCrawlerEditorMock> editor;
    testing::NiceMock<StackOfViewsMock> stack;
    serio::qt::CrawlerStepEditorViewModel viewModel = serio::qt::CrawlerStepEditorViewModel(editor, stack);
    QSignalSpy existingStepSpy = QSignalSpy(&viewModel, &serio::qt::CrawlerStepEditorViewModel::isExistingStepChanged);
    QSignalSpy crawlerStepTypesSpy = QSignalSpy(&viewModel, &serio::qt::CrawlerStepEditorViewModel::crawlerStepTypesChanged);
    QSignalSpy descriptionSpy = QSignalSpy(&viewModel, &serio::qt::CrawlerStepEditorViewModel::descriptionChanged);
    QSignalSpy propertiesSpy = QSignalSpy(&viewModel, &serio::qt::CrawlerStepEditorViewModel::propertiesChanged);
    virtual void SetUp() {
        ON_CALL(editor, getCrawlerStepTypes()).WillByDefault(::testing::Return(types));
        ON_CALL(editor, getCrawlerSteps()).WillByDefault(::testing::Return(steps));
    }
    void expectTypeOptionsDisplayed(unsigned int selectedTypeIndex) {
        auto typeOptions = viewModel.getCrawlerStepTypes();
        EXPECT_EQ(QString::fromStdString(types[0].getName()), typeOptions[0]->getName());
        EXPECT_EQ(QString::fromStdString(types[1].getName()), typeOptions[1]->getName());
        EXPECT_TRUE(typeOptions[selectedTypeIndex]->isChecked());
        EXPECT_FALSE(typeOptions[1 - selectedTypeIndex]->isChecked());
        EXPECT_EQ(2, crawlerStepTypesSpy.count());
    }
    void expectDescriptionDisplayed(unsigned int selectedTypeIndex) {
        EXPECT_EQ(QString::fromStdString(types[selectedTypeIndex].getDescription()), viewModel.getDescription());
        EXPECT_LE(1, descriptionSpy.count());
    }
    void expectPropertiesDisplayed(unsigned int typeIndex, const serio::core::CrawlerStep& selectedStep) {
        auto properties = viewModel.getProperties();
        auto expectedProperties = types[typeIndex].getMandatoryProperties();
        for (int i = 0; i < expectedProperties.size(); i++) {
            auto property = expectedProperties[i];
            EXPECT_EQ(QString::fromStdString(property), properties[i]->getName());
            EXPECT_EQ(QString::fromStdString(selectedStep.getPropertyOrFail(property)), properties[i]->getValue());
        }
        EXPECT_EQ(2, propertiesSpy.count());
    }
    void expectActionsToBeDefined(bool isDeleteEnabled) {
        auto actions = viewModel.getActions();
        ASSERT_EQ(isDeleteEnabled ? 3 : 2, actions.size());
        EXPECT_EQ(serio::qt::ButtonModel("cancel", serio::qt::ActionType::BACK, {}, false), *actions[0]);
        EXPECT_EQ(serio::qt::ButtonModel("save", serio::qt::ActionType::SAVE_CRAWLER_STEP), *actions[1]);
        if (isDeleteEnabled) {
            EXPECT_EQ(serio::qt::ButtonModel("delete", serio::qt::ActionType::REMOVE_CRAWLER_STEP), *actions[2]);
        }
    }
};

TEST_F(CrawlerStepEditorViewModelTest, shouldNotDisplayAnyInformation) {
    EXPECT_FALSE(viewModel.isExistingStep());
    EXPECT_TRUE(viewModel.getCrawlerStepTypes().isEmpty());
    EXPECT_TRUE(viewModel.getProperties().isEmpty());
    EXPECT_TRUE(viewModel.getDescription().isEmpty());
}

TEST_F(CrawlerStepEditorViewModelTest, newStepEditorShouldNotBeEditingExistingStep) {
    viewModel.openNew();
    EXPECT_FALSE(viewModel.isExistingStep());
}

TEST_F(CrawlerStepEditorViewModelTest, shouldOpenViewWhenEditingNewStep) {
    EXPECT_CALL(stack, pushView(serio::qt::crawlerStepEditorView));
    viewModel.openNew();
    EXPECT_EQ(1, existingStepSpy.count());
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayExistingCrawlerStepTypesWhilePreselectingFirstOneOfThem) {
    viewModel.openNew();
    viewModel.load();
    expectTypeOptionsDisplayed(0);
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayDescriptionOfPreselectedFirstCrawlerStepType) {
    viewModel.openNew();
    viewModel.load();
    expectDescriptionDisplayed(0);
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayEmptyPropertiesOfPreselectedFirstCrawlerStepType) {
    viewModel.openNew();
    viewModel.load();
    expectPropertiesDisplayed(0, serio::core::CrawlerStep("type 1", {{"property 1", ""}, {"property 2", ""}}));
}

TEST_F(CrawlerStepEditorViewModelTest, existingStepEditorShouldBeEditingExistingStep) {
    viewModel.openExisting(QVariantList({1}));
    EXPECT_TRUE(viewModel.isExistingStep());
}

TEST_F(CrawlerStepEditorViewModelTest, shouldOpenViewWhenEditingExistingStep) {
    EXPECT_CALL(stack, pushView(serio::qt::crawlerStepEditorView));
    viewModel.openExisting(QVariantList({1}));
    EXPECT_EQ(1, existingStepSpy.count());
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayExistingCrawlerStepTypesWhilePreselectingEditedOneOfThem) {
    viewModel.openExisting(QVariantList({0}));
    viewModel.load();
    expectTypeOptionsDisplayed(1);
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayDescriptionOfPreselectedEditedCrawlerStepType) {
    viewModel.openExisting(QVariantList({0}));
    viewModel.load();
    expectDescriptionDisplayed(1);
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayPropertiesOfEditedStep) {
    viewModel.openExisting(QVariantList({1}));
    viewModel.load();
    expectPropertiesDisplayed(0, steps[1]);
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayNoPropertiesSinceEditedStepHasNone) {
    viewModel.openExisting(QVariantList({0}));
    viewModel.load();
    EXPECT_TRUE(viewModel.getProperties().isEmpty());
    EXPECT_EQ(2, propertiesSpy.count());
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayCrawlerStepTypesWithSelectedTypeChecked) {
    viewModel.openNew();
    viewModel.load();
    viewModel.selectType(QVariantList({QString::fromStdString(types[1].getName())}));
    expectTypeOptionsDisplayed(1);
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayDescriptionOfSelectedCrawlerType) {
    viewModel.openNew();
    viewModel.load();
    QSignalSpy checkedSpy(viewModel.getCrawlerStepTypes()[1], &serio::qt::RadioButtonModel::checkedChanged);
    viewModel.selectType(QVariantList({QString::fromStdString(types[1].getName())}));
    expectDescriptionDisplayed(1);
    EXPECT_EQ(1, checkedSpy.count());
}

TEST_F(CrawlerStepEditorViewModelTest, shouldAddNewStepWithEmptyProperties) {
    serio::core::CrawlerStep expectedStep("type 1", {{"property 1", ""}, {"property 2", ""}});
    EXPECT_CALL(editor, addCrawlerStep(expectedStep));
    EXPECT_CALL(stack, popCurrentView());
    viewModel.openNew();
    viewModel.load();
    viewModel.selectType(QVariantList({"type 1"}));
    viewModel.save();
}

TEST_F(CrawlerStepEditorViewModelTest, shouldAddNewStepWithPropertiesSpecified) {
    auto expectedStep = steps[1];
    EXPECT_CALL(editor, addCrawlerStep(expectedStep));
    EXPECT_CALL(stack, popCurrentView());
    viewModel.openNew();
    viewModel.load();
    viewModel.selectType(QVariantList({QString::fromStdString(expectedStep.getType())}));
    auto properties = viewModel.getProperties();
    properties[0]->setValue(QString::fromStdString(expectedStep.getPropertyOrFail("property 1")));
    properties[1]->setValue(QString::fromStdString(expectedStep.getPropertyOrFail("property 2")));
    viewModel.save();
}

TEST_F(CrawlerStepEditorViewModelTest, shouldReplaceExistingStepWithEditedOne) {
    auto editedStepIndex = 1;
    auto updatedStep = steps[0];
    EXPECT_CALL(editor, replaceCrawlerStep(editedStepIndex, updatedStep));
    EXPECT_CALL(stack, popCurrentView());
    viewModel.openExisting(QVariantList({editedStepIndex}));
    viewModel.load();
    viewModel.selectType(QVariantList({QString::fromStdString(updatedStep.getType())}));
    viewModel.save();
}

TEST_F(CrawlerStepEditorViewModelTest, shouldRemoveExistingStep) {
    EXPECT_CALL(editor, removeCrawlerStep(0));
    EXPECT_CALL(stack, popCurrentView());
    viewModel.openExisting(QVariantList({0}));
    viewModel.load();
    viewModel.remove();
}

TEST_F(CrawlerStepEditorViewModelTest, shouldReturnListOfActionsForEditingNewStep) {
    viewModel.openNew();
    expectActionsToBeDefined(false);
}

TEST_F(CrawlerStepEditorViewModelTest, shouldRepopulateListOfActionsEveryTimeNewStepGetsEdited) {
    viewModel.openNew();
    viewModel.openNew();
    EXPECT_EQ(2, viewModel.getActions().size());
}

TEST_F(CrawlerStepEditorViewModelTest, shouldNotifyWatchersAboutActionsChange) {
    QSignalSpy spy(&viewModel, &serio::qt::CrawlerStepEditorViewModel::actionsChanged);
    viewModel.openNew();
    EXPECT_EQ(2, spy.count());
}

TEST_F(CrawlerStepEditorViewModelTest, shouldReturnListOfActionsForEditingExistingStep) {
    viewModel.openExisting(QVariantList({0}));
    expectActionsToBeDefined(true);
}

TEST_F(CrawlerStepEditorViewModelTest, shouldRepopulateListOfActionsEveryTimeExistingtepGetsEdited) {
    viewModel.openExisting(QVariantList({0}));
    viewModel.openExisting(QVariantList({0}));
    EXPECT_EQ(3, viewModel.getActions().size());
}