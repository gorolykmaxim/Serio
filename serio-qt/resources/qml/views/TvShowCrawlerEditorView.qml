import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import Serio 1.0
import "../widgets"

CenteredViewLayout {
    onCreated: {
        tvShowNameEdt.textEdited.connect(() => actionRouter.trigger(ActionType.SET_TV_SHOW_NAME, [tvShowNameEdt.text]))
        saveBtn.clicked.connect(() => actionRouter.trigger(ActionType.SAVE_TV_SHOW_CRAWLER, []))
        cancelBtn.clicked.connect(() => actionRouter.trigger(ActionType.BACK, []))
        configureEpisodeVideoCrawlerBtn.clicked.connect(() => actionRouter.trigger(ActionType.OPEN_CRAWLER_EDITOR, [crawlerEditorViewModel.episodeVideoCrawlerType]))
        configureThumbnailCrawlerBtn.clicked.connect(() => actionRouter.trigger(ActionType.OPEN_CRAWLER_EDITOR, [crawlerEditorViewModel.thumbnailCrawlerType]))
        configureEpisodeNamesCrawlerBtn.clicked.connect(() => actionRouter.trigger(ActionType.OPEN_CRAWLER_EDITOR, [crawlerEditorViewModel.episodeNameCrawlerType]))
    }
    onDisplayed: actionRouter.trigger(ActionType.LOAD_TV_SHOW_CRAWLER_EDITOR_TV_SHOW_NAME, [])
    AccentTitle {
        Layout.fillWidth: true
        text: "Configure TV Show Crawler"
    }
    TextField {
        id: tvShowNameEdt
        Layout.fillWidth: true
        focus: tvShowCrawlerEditorViewModel.canTvShowNameBeChanged
        enabled: tvShowCrawlerEditorViewModel.canTvShowNameBeChanged
        text: tvShowCrawlerEditorViewModel.tvShowName
        placeholderText: "TV Show name"
        KeyNavigation.tab: configureEpisodeVideoCrawlerBtn
        KeyNavigation.down: configureEpisodeVideoCrawlerBtn
    }
    SerioButton {
        id: configureEpisodeVideoCrawlerBtn
        Layout.fillWidth: true
        text: "configure episode video crawler"
        KeyNavigation.tab: configureThumbnailCrawlerBtn
        KeyNavigation.down: configureThumbnailCrawlerBtn
    }
    SerioButton {
        id: configureThumbnailCrawlerBtn
        Layout.fillWidth: true
        text: "configure thumbnail crawler"
        KeyNavigation.tab: configureEpisodeNamesCrawlerBtn
        KeyNavigation.down: configureEpisodeNamesCrawlerBtn
    }
    SerioButton {
        id: configureEpisodeNamesCrawlerBtn
        Layout.fillWidth: true
        text: "configure episode name crawler"
        KeyNavigation.tab: cancelBtn
        KeyNavigation.down: cancelBtn
    }
    RightToLeftButtonRow {
        Layout.fillWidth: true
        spacing: globalPadding
        SerioButton {
            id: saveBtn
            enabled: tvShowCrawlerEditorViewModel.canCrawlerBeSaved
            text: "save"
            KeyNavigation.tab: tvShowNameEdt
            KeyNavigation.up: configureEpisodeNamesCrawlerBtn
        }
        SerioButton {
            id: cancelBtn
            text: "cancel"
            focus: !tvShowCrawlerEditorViewModel.canTvShowNameBeChanged
            KeyNavigation.tab: saveBtn
            KeyNavigation.right: saveBtn
        }
    }
}
