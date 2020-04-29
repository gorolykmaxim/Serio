package org.serio.core.applicationcontroller.tasks.crawllog;

import org.serio.core.applicationcontroller.event.CrawlLogEntryDetailsEvent;
import org.serio.core.applicationcontroller.event.CrawlLogEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.model.IndexedCrawlLogEntry;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.userinterface.UserInterface;

import java.util.Optional;

public class ViewLogEntryTask implements ControllerTask {
    private final int logEntryId;

    public ViewLogEntryTask(int logEntryId) {
        this.logEntryId = logEntryId;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        Optional<CrawlLogEvent> possibleLastEvent = eventStack.peek(CrawlLogEvent.class);
        if (possibleLastEvent.isPresent()) {
            CrawlLogEvent lastEvent = possibleLastEvent.get();
            IndexedCrawlLogEntry entry = lastEvent.getLogEntryById(logEntryId)
                    .orElseThrow(() -> new IllegalArgumentException(String.format("Crawl log entry with ID '%d' does not exist", logEntryId)));
            CrawlLogEntryDetailsEvent event = new CrawlLogEntryDetailsEvent(entry);
            eventStack.push(event);
            userInterface.sendEvent(event);
        }
    }
}