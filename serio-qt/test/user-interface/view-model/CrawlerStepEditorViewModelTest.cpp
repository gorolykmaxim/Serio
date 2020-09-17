#include <gtest/gtest.h>
#include <TvShowCrawlerEditorMock.h>
#include <user-interface/StackOfViews.h>
#include <user-interface/view-model/CrawlerStepEditorViewModel.h>
#include <QSignalSpy>

class CrawlerStepEditorViewModelTest : public ::testing::Test {
protected:
    std::vector<serio::core::CrawlerStepType> types = {
        serio::core::CrawlerStepType("type 1", "description 1", {"property 1", "property 2"}),
        serio::core::CrawlerStepType("type 2", "description 2"),
    };
    std::vector<serio::core::CrawlerStep> steps {
        serio::core::CrawlerStep("type 2"),
        serio::core::CrawlerStep("type 1", {{"property 1", "value 1"}, {"property 2", "value 2"}})
    };
    TvShowCrawlerEditorMock editor = TvShowCrawlerEditorMock::create();
    serio::qt::StackOfViews stack;
    serio::qt::CrawlerStepEditorViewModel viewModel = serio::qt::CrawlerStepEditorViewModel(editor, stack);
    QSignalSpy stackPushSpy = QSignalSpy(&stack, &serio::qt::StackOfViews::push);
    QSignalSpy existingStepSpy = QSignalSpy(&viewModel, &serio::qt::CrawlerStepEditorViewModel::isExistingStepChanged);
    QSignalSpy crawlerStepTypesSpy = QSignalSpy(&viewModel, &serio::qt::CrawlerStepEditorViewModel::crawlerStepTypesChanged);
    QSignalSpy descriptionSpy = QSignalSpy(&viewModel, &serio::qt::CrawlerStepEditorViewModel::descriptionChanged);
    QSignalSpy propertiesSpy = QSignalSpy(&viewModel, &serio::qt::CrawlerStepEditorViewModel::propertiesChanged);
    QSignalSpy stackPopSpy = QSignalSpy(&stack, &serio::qt::StackOfViews::pop);
    virtual void TearDown() {
        if (!viewModel.getCrawlerStepTypes().isEmpty()) {
            qDeleteAll(viewModel.getCrawlerStepTypes());
        }
        if (!viewModel.getProperties().isEmpty()) {
            qDeleteAll(viewModel.getProperties());
        }
    }
    void expectViewDisplayed() {
        EXPECT_EQ(1, existingStepSpy.count());
        ASSERT_EQ(1, stackPushSpy.count());
        QVariantList args = stackPushSpy.takeFirst();
        EXPECT_EQ(QStringList("views/CrawlerStepEditorView.qml"), args[0].toStringList());
    }
    void expectViewPoppedFromStack() {
        ASSERT_EQ(1, stackPopSpy.count());
        QVariantList args = stackPopSpy.takeFirst();
        EXPECT_TRUE(args[0].toBool());
    }
    void expectTypeOptionsDisplayed(unsigned int selectedTypeIndex) {
        QList<serio::qt::RadioButtonModel*> typeOptions = viewModel.getCrawlerStepTypes();
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
        QList<serio::qt::TextFieldModel*> properties = viewModel.getProperties();
        std::vector<std::string> expectedProperties = types[typeIndex].getMandatoryProperties();
        for (int i = 0; i < expectedProperties.size(); i++) {
            std::string property = expectedProperties[i];
            EXPECT_EQ(QString::fromStdString(property), properties[i]->getName());
            EXPECT_EQ(QString::fromStdString(selectedStep.getPropertyOrFail(property)), properties[i]->getValue());
        }
        EXPECT_EQ(2, propertiesSpy.count());
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
    viewModel.openNew();
    expectViewDisplayed();
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayExistingCrawlerStepTypesWhilePreselectingFirstOneOfThem) {
    EXPECT_CALL(editor, getCrawlerStepTypes()).WillOnce(::testing::Return(types));
    viewModel.openNew();
    viewModel.load();
    expectTypeOptionsDisplayed(0);
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayDescriptionOfPreselectedFirstCrawlerStepType) {
    EXPECT_CALL(editor, getCrawlerStepTypes()).WillOnce(::testing::Return(types));
    viewModel.openNew();
    viewModel.load();
    expectDescriptionDisplayed(0);
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayEmptyPropertiesOfPreselectedFirstCrawlerStepType) {
    EXPECT_CALL(editor, getCrawlerStepTypes()).WillOnce(::testing::Return(types));
    viewModel.openNew();
    viewModel.load();
    expectPropertiesDisplayed(0, serio::core::CrawlerStep("type 1", {{"property 1", ""}, {"property 2", ""}}));
}

TEST_F(CrawlerStepEditorViewModelTest, existingStepEditorShouldBeEditingExistingStep) {
    viewModel.openExisting(QVariantList({1}));
    EXPECT_TRUE(viewModel.isExistingStep());
}

TEST_F(CrawlerStepEditorViewModelTest, shouldOpenViewWhenEditingExistingStep) {
    viewModel.openExisting(QVariantList({1}));
    expectViewDisplayed();
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayExistingCrawlerStepTypesWhilePreselectingEditedOneOfThem) {
    EXPECT_CALL(editor, getCrawlerSteps()).WillOnce(::testing::Return(steps));
    EXPECT_CALL(editor, getCrawlerStepTypes()).WillOnce(::testing::Return(types));
    viewModel.openExisting(QVariantList({0}));
    viewModel.load();
    expectTypeOptionsDisplayed(1);
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayDescriptionOfPreselectedEditedCrawlerStepType) {
    EXPECT_CALL(editor, getCrawlerSteps()).WillOnce(::testing::Return(steps));
    EXPECT_CALL(editor, getCrawlerStepTypes()).WillOnce(::testing::Return(types));
    viewModel.openExisting(QVariantList({0}));
    viewModel.load();
    expectDescriptionDisplayed(1);
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayPropertiesOfEditedStep) {
    EXPECT_CALL(editor, getCrawlerSteps()).WillOnce(::testing::Return(steps));
    EXPECT_CALL(editor, getCrawlerStepTypes()).WillOnce(::testing::Return(types));
    viewModel.openExisting(QVariantList({1}));
    viewModel.load();
    expectPropertiesDisplayed(0, steps[1]);
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayNoPropertiesSinceEditedStepHasNone) {
    EXPECT_CALL(editor, getCrawlerSteps()).WillOnce(::testing::Return(steps));
    EXPECT_CALL(editor, getCrawlerStepTypes()).WillOnce(::testing::Return(types));
    viewModel.openExisting(QVariantList({0}));
    viewModel.load();
    EXPECT_TRUE(viewModel.getProperties().isEmpty());
    EXPECT_EQ(2, propertiesSpy.count());
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayCrawlerStepTypesWithSelectedTypeChecked) {
    EXPECT_CALL(editor, getCrawlerStepTypes()).WillOnce(::testing::Return(types));
    viewModel.openNew();
    viewModel.load();
    viewModel.selectType(QVariantList({QString::fromStdString(types[1].getName())}));
    expectTypeOptionsDisplayed(1);
}

TEST_F(CrawlerStepEditorViewModelTest, shouldDisplayDescriptionOfSelectedCrawlerType) {
    EXPECT_CALL(editor, getCrawlerStepTypes()).WillOnce(::testing::Return(types));
    viewModel.openNew();
    viewModel.load();
    QSignalSpy checkedSpy(viewModel.getCrawlerStepTypes()[1], &serio::qt::RadioButtonModel::checkedChanged);
    viewModel.selectType(QVariantList({QString::fromStdString(types[1].getName())}));
    expectDescriptionDisplayed(1);
    EXPECT_EQ(1, checkedSpy.count());
}

TEST_F(CrawlerStepEditorViewModelTest, shouldAddNewStepWithEmptyProperties) {
    serio::core::CrawlerStep expectedStep("type 1", {{"property 1", ""}, {"property 2", ""}});
    EXPECT_CALL(editor, getCrawlerStepTypes()).WillOnce(::testing::Return(types));
    EXPECT_CALL(editor, addCrawlerStep(expectedStep));
    viewModel.openNew();
    viewModel.load();
    viewModel.selectType(QVariantList({"type 1"}));
    viewModel.save();
    expectViewPoppedFromStack();
}

TEST_F(CrawlerStepEditorViewModelTest, shouldAddNewStepWithPropertiesSpecified) {
    serio::core::CrawlerStep expectedStep = steps[1];
    EXPECT_CALL(editor, getCrawlerStepTypes()).WillOnce(::testing::Return(types));
    EXPECT_CALL(editor, addCrawlerStep(expectedStep));
    viewModel.openNew();
    viewModel.load();
    viewModel.selectType(QVariantList({QString::fromStdString(expectedStep.getType())}));
    QList<serio::qt::TextFieldModel*> properties = viewModel.getProperties();
    properties[0]->setValue(QString::fromStdString(expectedStep.getPropertyOrFail("property 1")));
    properties[1]->setValue(QString::fromStdString(expectedStep.getPropertyOrFail("property 2")));
    viewModel.save();
    expectViewPoppedFromStack();
}

TEST_F(CrawlerStepEditorViewModelTest, shouldReplaceExistingStepWithEditedOne) {
    unsigned int editedStepIndex = 1;
    serio::core::CrawlerStep updatedStep = steps[0];
    EXPECT_CALL(editor, getCrawlerSteps()).WillOnce(::testing::Return(steps));
    EXPECT_CALL(editor, getCrawlerStepTypes()).WillOnce(::testing::Return(types));
    EXPECT_CALL(editor, replaceCrawlerStep(editedStepIndex, updatedStep));
    viewModel.openExisting(QVariantList({editedStepIndex}));
    viewModel.load();
    viewModel.selectType(QVariantList({QString::fromStdString(updatedStep.getType())}));
    viewModel.save();
    expectViewPoppedFromStack();
}

TEST_F(CrawlerStepEditorViewModelTest, shouldRemoveExistingStep) {
    EXPECT_CALL(editor, getCrawlerSteps()).WillOnce(::testing::Return(steps));
    EXPECT_CALL(editor, getCrawlerStepTypes()).WillOnce(::testing::Return(types));
    EXPECT_CALL(editor, removeCrawlerStep(0));
    viewModel.openExisting(QVariantList({0}));
    viewModel.load();
    viewModel.remove();
    expectViewPoppedFromStack();
}
