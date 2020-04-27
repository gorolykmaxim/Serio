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
        Optional<EditShowCrawlerEvent> possibleLastEvent = eventStack.pop(EditShowCrawlerEvent.class);
        if (possibleLastEvent.isPresent()) {
            EditShowCrawlerEvent lastEvent = possibleLastEvent.get();
            try {
                userInterface.sendEvent(new CrawlingInProgressEvent());
                Show show = showsCrawler.crawlShowAndSaveCrawler(
                        StringUtils.defaultString(showName, lastEvent.getShowName().orElse(null)),
                        lastEvent.getCrawler(CrawlerTypes.THUMBNAIL).orElse(null),
                        lastEvent.getCrawler(CrawlerTypes.EPISODE_VIDEO).orElse(null),
                        lastEvent.getCrawler(CrawlerTypes.EPISODE_NAME).orElse(null)
                );
                shows.saveShow(show);
                if (eventStack.pop(AllShowsEvent.class).isPresent() || eventStack.pop(ShowDetailsEvent.class).isPresent()) {
                    new SelectShowTask(show.getId().toString(), shows, dateFormat).execute(eventStack, userInterface);
                } else {
                    throw new IllegalEventStackStateException(lastEvent, eventStack);
                }
            } catch (Exception e) {
                // In case the crawl fails or anything else we will push the edit show crawler event back to stack,
                // so that when the user will close the error dialog - the edit show crawler view will be displayed
                // back with all the latest changes made by user.
                eventStack.push(lastEvent);
                throw e;
            }
        }
    }
}
