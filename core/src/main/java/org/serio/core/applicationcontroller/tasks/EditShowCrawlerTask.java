package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.EditShowCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.showscrawler.SerializedShowCrawlerParts;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.userinterface.UserInterface;

public class EditShowCrawlerTask implements ControllerTask {
    private final ShowsCrawler showsCrawler;

    public EditShowCrawlerTask(ShowsCrawler showsCrawler) {
        this.showsCrawler = showsCrawler;
    }

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
