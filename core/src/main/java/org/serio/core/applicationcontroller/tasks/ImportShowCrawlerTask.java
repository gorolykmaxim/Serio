package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.CrawlingInProgressEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ImportShowFromJsonEvent;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.shows.Shows;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.showstorage.Show;
import org.serio.core.userinterface.UserInterface;

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
        if (eventStack.isLastEventOfType(ImportShowFromJsonEvent.class)) {
            userInterface.sendEvent(new CrawlingInProgressEvent());
            Show show = showsCrawler.crawlShowAndSaveCrawler(rawShowCrawler);
            shows.saveShow(show);
            eventStack.pop(ImportShowFromJsonEvent.class);
            new SelectShowTask(show.getId().toString(), shows, dateFormat).execute(eventStack, userInterface);
        }
    }
}
