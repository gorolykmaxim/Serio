package org.serio.core.applicationcontroller.tasks;

import org.apache.commons.lang3.StringUtils;
import org.serio.core.applicationcontroller.event.*;
import org.serio.core.applicationcontroller.model.CrawlerTypes;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.shows.Shows;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.showstorage.Show;
import org.serio.core.userinterface.UserInterface;

import java.util.Optional;

public class SaveShowCrawlerTask implements ControllerTask {
    private final String showName;
    private final Shows shows;
    private final ShowsCrawler showsCrawler;
    private final DateFormat dateFormat;

    public SaveShowCrawlerTask(String showName, Shows shows, ShowsCrawler showsCrawler, DateFormat dateFormat) {
        this.showName = showName;
        this.shows = shows;
        this.showsCrawler = showsCrawler;
        this.dateFormat = dateFormat;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(EditShowCrawlerEvent.class).ifPresent(lastEvent -> {
            userInterface.sendEvent(new CrawlingInProgressEvent());
            Show show = showsCrawler.crawlShowAndSaveCrawler(
                    StringUtils.defaultString(showName, lastEvent.getShowName().orElse(null)),
                    lastEvent.getCrawler(CrawlerTypes.THUMBNAIL).orElse(null),
                    lastEvent.getCrawler(CrawlerTypes.EPISODE_VIDEO).orElse(null),
                    lastEvent.getCrawler(CrawlerTypes.EPISODE_NAME).orElse(null)
            );
            shows.saveShow(show);
            eventStack.pop(EditShowCrawlerEvent.class);
            if (eventStack.isLastEventOfType(AllShowsEvent.class) || eventStack.pop(ShowDetailsEvent.class).isPresent()) {
                new SelectShowTask(show.getId().toString(), shows, dateFormat).execute(eventStack, userInterface);
            } else {
                throw new IllegalEventStackStateException(lastEvent, eventStack);
            }
        });
    }
}
