package org.serio.core.applicationcontroller.tasks.importshow;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ImportShowFromJsonEvent;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.shows.Shows;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.userinterface.UserInterface;

public class ImportShowCrawlerWithoutOverrideTask implements ControllerTask {
    private final String rawShowCrawler;
    private final Shows shows;
    private final ShowsCrawler showsCrawler;
    private final DateFormat dateFormat;

    public ImportShowCrawlerWithoutOverrideTask(String rawShowCrawler, Shows shows, ShowsCrawler showsCrawler, DateFormat dateFormat) {
        this.rawShowCrawler = rawShowCrawler;
        this.shows = shows;
        this.showsCrawler = showsCrawler;
        this.dateFormat = dateFormat;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        if (eventStack.isLastEventOfType(ImportShowFromJsonEvent.class)) {
            String showName = showsCrawler.getShowNameDefinedInShowCrawler(rawShowCrawler);
            if (shows.doesShowWithNameExists(showName)) {
                eventStack.pop(ImportShowFromJsonEvent.class);
                eventStack.push(new ImportShowFromJsonEvent(rawShowCrawler));
                ShowDialogEvent event = ShowDialogEvent.showOverrideDialog(showName);
                eventStack.push(event);
                userInterface.sendEvent(event);
            } else {
                new ImportShowCrawlerTask(rawShowCrawler, shows, showsCrawler, dateFormat).execute(eventStack, userInterface);
            }
        }
    }
}
