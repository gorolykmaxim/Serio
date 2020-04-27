package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.CrawlingInProgressEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ImportShowFromJsonEvent;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.shows.Shows;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.showstorage.Show;
import org.serio.core.userinterface.UserInterface;

import java.util.Optional;

public class ImportShowCrawlerTask implements ControllerTask {
    private final String rawShowCrawler;
    private final Shows shows;
    private final ShowsCrawler showsCrawler;
    private final DateFormat dateFormat;

    public ImportShowCrawlerTask(String rawShowCrawler, Shows shows, ShowsCrawler showsCrawler, DateFormat dateFormat) {
        this.rawShowCrawler = rawShowCrawler;
        this.shows = shows;
        this.showsCrawler = showsCrawler;
        this.dateFormat = dateFormat;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        Optional<ImportShowFromJsonEvent> possibleLastEvent = eventStack.pop(ImportShowFromJsonEvent.class);
        if (possibleLastEvent.isPresent()) {
            try {
                userInterface.sendEvent(new CrawlingInProgressEvent());
                Show show = showsCrawler.crawlShowAndSaveCrawler(rawShowCrawler);
                shows.saveShow(show);
                new SelectShowTask(show.getId().toString(), shows, dateFormat).execute(eventStack, userInterface);
            } catch (Exception e) {
                // Similar to SaveShowCrawlerTask handling. See details there.
                eventStack.push(possibleLastEvent.get());
                throw e;
            }
        }
    }
}
