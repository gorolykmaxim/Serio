package org.serio.core.applicationcontroller.tasks.showdetails;

import org.serio.core.applicationcontroller.event.EditShowCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.showscrawler.SerializedShowCrawlerParts;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.userinterface.UserInterface;

/**
 * Open the {@link org.serio.core.userinterface.ViewIds#EDIT_SHOW_CRAWLER} to edit the crawler of the currently
 * selected show.
 */
public class EditShowCrawlerTask implements ControllerTask {
    private final ShowsCrawler showsCrawler;

    /**
     * Construct a task.
     *
     * @param showsCrawler module that will be used to obtain configuration of the crawler of the selected show
     */
    public EditShowCrawlerTask(ShowsCrawler showsCrawler) {
        this.showsCrawler = showsCrawler;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(ShowDetailsEvent.class).ifPresent(lastEvent -> {
            DisplayableShow show = lastEvent.getShow();
            SerializedShowCrawlerParts parts = showsCrawler.getSerializedPartsOfCrawlerOfShow(show.getId().toString());
            EditShowCrawlerEvent event = new EditShowCrawlerEvent(
                    show.getId(),
                    parts.getShowName(),
                    parts.getSerializedThumbnailCrawler(),
                    parts.getSerializedEpisodeVideoCrawler(),
                    parts.getSerializedEpisodeNameCrawler()
            );
            eventStack.push(event);
            userInterface.sendEvent(event);
        });
    }
}
