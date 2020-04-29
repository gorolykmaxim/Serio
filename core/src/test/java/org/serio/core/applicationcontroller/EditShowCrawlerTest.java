package org.serio.core.applicationcontroller;

import org.junit.Before;
import org.junit.Test;
import org.serio.core.applicationcontroller.event.EditCrawlerEvent;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.applicationcontroller.model.CrawlerTypes;
import org.serio.core.shows.WatchableShow;
import org.serio.core.userinterface.ViewIds;

import static org.junit.Assert.assertEquals;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.reset;
import static org.mockito.Mockito.when;

public class EditShowCrawlerTest extends BaseApplicationControllerTest {
    @Override
    @Before
    public void setUp() throws Exception {
        super.setUp();
        applicationController.viewAllShows();
        applicationController.addShow();
        reset(userInterface);
    }

    @Override
    public void shouldEditThumbnailCrawler() {
        // when
        applicationController.editThumbnailCrawler();
        applicationController.editThumbnailCrawler();
        // then
        assertEditCrawlerEvent(captureLastUserInterfaceEvent(EditCrawlerEvent.class), CrawlerTypes.THUMBNAIL, "");
    }

    @Test
    public void shouldEditExistingThumbnailCrawler() {
        // given
        editExistingShowCrawler();
        // when
        applicationController.editThumbnailCrawler();
        applicationController.editThumbnailCrawler();
        // then
        assertEditCrawlerEvent(captureLastUserInterfaceEvent(EditCrawlerEvent.class), CrawlerTypes.THUMBNAIL, rawCrawler);
    }

    @Override
    public void shouldEditEpisodeVideoCrawler() {
        // when
        applicationController.editEpisodeVideoCrawler();
        applicationController.editEpisodeVideoCrawler();
        // then
        assertEditCrawlerEvent(captureLastUserInterfaceEvent(EditCrawlerEvent.class), CrawlerTypes.EPISODE_VIDEO, "");
    }

    @Test
    public void shouldEditExistingEpisodeVideoCrawler() {
        // given
        editExistingShowCrawler();
        // when
        applicationController.editEpisodeVideoCrawler();
        applicationController.editEpisodeVideoCrawler();
        // then
        assertEditCrawlerEvent(captureLastUserInterfaceEvent(EditCrawlerEvent.class), CrawlerTypes.EPISODE_VIDEO, rawCrawler);
    }

    @Override
    public void shouldEditEpisodeNameCrawler() {
        // when
        applicationController.editEpisodeNameCrawler();
        applicationController.editEpisodeNameCrawler();
        // then
        assertEditCrawlerEvent(captureLastUserInterfaceEvent(EditCrawlerEvent.class), CrawlerTypes.EPISODE_NAME, "");
    }

    @Test
    public void shouldEditExistingEpisodeNameCrawler() {
        // given
        editExistingShowCrawler();
        // when
        applicationController.editEpisodeNameCrawler();
        applicationController.editEpisodeNameCrawler();
        // then
        assertEditCrawlerEvent(captureLastUserInterfaceEvent(EditCrawlerEvent.class), CrawlerTypes.EPISODE_NAME, rawCrawler);
    }

    @Override
    public void shouldSaveShowCrawler() {
        // given
        WatchableShow show = shows.findShowById(friends);
        applicationController.editEpisodeVideoCrawler();
        applicationController.saveCrawler(rawCrawler);
        reset(userInterface);
        // when
        applicationController.saveShowCrawler(show.getName());
        applicationController.saveShowCrawler(show.getName());
        // then
        assertShowCrawled(friends, "2 days ago");
    }

    @Test
    public void shouldNotSaveShowCrawlerWithShowNameThatIsAlreadyTaken() {
        // given
        String showName = setUpShowNameConflict();
        applicationController.editEpisodeVideoCrawler();
        applicationController.saveCrawler(rawCrawler);
        reset(userInterface);
        // when
        applicationController.saveShowCrawler(showName);
        // then
        ShowDialogEvent event = captureLastUserInterfaceEvent(ShowDialogEvent.class);
        assertEquals(ViewIds.SHOW_OVERRIDE_DIALOG, event.getViewId());
        assertEquals(showName, event.getShowName());
    }

    @Test
    public void shouldSaveShowCrawlerWithATakenShowNameAfterUsersOverrideConfirmation() {
        // given
        String showName = setUpShowNameConflict();
        applicationController.editEpisodeVideoCrawler();
        applicationController.saveCrawler(rawCrawler);
        applicationController.saveShowCrawler(showName);
        reset(userInterface);
        // when
        applicationController.confirmShowOverride();
        applicationController.confirmShowOverride();
        // then
        assertShowCrawled(friends, "2 days ago");
    }

    @Test
    public void shouldUpdateCrawlerOfTheSelectedShowAndReCrawlTheShow() {
        // given
        String showName = setUpShowNameConflict();
        editExistingShowCrawler();
        applicationController.editEpisodeVideoCrawler();
        applicationController.saveCrawler(rawCrawler);
        reset(userInterface);
        // when
        applicationController.saveShowCrawler(showName);
        applicationController.saveShowCrawler(showName);
        // then
        assertShowCrawled(friends, "2 days ago");
    }

    @Test
    public void shouldFailToSaveShowCrawler() {
        // given
        WatchableShow show = shows.findShowById(friends);
        when(showsCrawler.crawlShowAndSaveCrawler(any(), any(), any(), any())).thenThrow(expectedException);
        applicationController.editEpisodeVideoCrawler();
        applicationController.saveCrawler(rawCrawler);
        reset(userInterface);
        // when
        applicationController.saveShowCrawler(show.getName());
        applicationController.saveShowCrawler(show.getName());
        // then
        assertCrawlingErrorReceived();
    }

    @Override
    public void shouldBack() {
        // when
        applicationController.back();
        // then
        assertCurrentView(ViewIds.ALL_SHOWS);
    }

    @Test
    public void shouldBackToShowDetails() {
        // given
        editExistingShowCrawler();
        reset(userInterface);
        // when
        applicationController.back();
        // then
        assertCurrentView(ViewIds.SHOW_DETAILS);
    }

    private void editExistingShowCrawler() {
        applicationController.back();
        applicationController.selectShow(friends.toString());
        applicationController.editShowCrawler();
        reset(userInterface);
    }

    private String setUpShowNameConflict() {
        WatchableShow show = shows.findShowById(friends);
        when(shows.doesShowWithNameExists(show.getName())).thenReturn(true);
        return show.getName();
    }

    private void assertEditCrawlerEvent(EditCrawlerEvent event, String expectedCrawlerType, String expectedCrawler) {
        assertEquals(ViewIds.EDIT_CRAWLER, event.getViewId());
        assertEquals(expectedCrawlerType, event.getCrawlerType());
        assertEquals(expectedCrawler, event.getCrawler());
    }
}