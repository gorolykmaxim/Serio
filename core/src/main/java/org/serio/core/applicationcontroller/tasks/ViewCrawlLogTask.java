package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.CrawlLogEvent;
import org.serio.core.applicationcontroller.event.CrawlPreviewEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.showcrawlerlogstorage.CrawlLogEntry;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.userinterface.UserInterface;

import java.util.Collections;
import java.util.List;
import java.util.Optional;

public class ViewCrawlLogTask implements ControllerTask {
    private final String showId;
    private final ShowsCrawler showsCrawler;

    public ViewCrawlLogTask(String showId, ShowsCrawler showsCrawler) {
        this.showId = showId;
        this.showsCrawler = showsCrawler;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        CrawlLogEvent event = null;
        Optional<CrawlPreviewEvent> possibleLastEvent = eventStack.pop(CrawlPreviewEvent.class);
        if (possibleLastEvent.isPresent()) {
            CrawlPreviewEvent lastEvent = possibleLastEvent.get();
            event = new CrawlLogEvent(lastEvent.getCrawlerType(), lastEvent.getCrawlingResult().getLog());
        } else if (showId != null && eventStack.isLastEventOfType(ShowDetailsEvent.class)) {
            List<CrawlLogEntry> lastCrawlingLog = showsCrawler.getLastCrawlingLogOfShow(showId)
                    .orElse(Collections.emptyList());
            event = new CrawlLogEvent(lastCrawlingLog);
        }
        if (event != null) {
            eventStack.push(event);
            userInterface.sendEvent(event);
        }
    }
}
