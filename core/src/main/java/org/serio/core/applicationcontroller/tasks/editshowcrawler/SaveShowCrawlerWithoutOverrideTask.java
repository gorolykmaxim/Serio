package org.serio.core.applicationcontroller.tasks.editshowcrawler;

import org.serio.core.applicationcontroller.event.EditShowCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.shows.Shows;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.userinterface.UserInterface;

public class SaveShowCrawlerWithoutOverrideTask implements ControllerTask {
    private final String showName;
    private final Shows shows;
    private final ShowsCrawler showsCrawler;
    private final DateFormat dateFormat;

    public SaveShowCrawlerWithoutOverrideTask(String showName, Shows shows, ShowsCrawler showsCrawler, DateFormat dateFormat) {
        this.showName = showName;
        this.shows = shows;
        this.showsCrawler = showsCrawler;
        this.dateFormat = dateFormat;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(EditShowCrawlerEvent.class).ifPresent(lastEvent -> {
            if (shows.doesShowWithNameExists(showName)) {
                eventStack.pop(EditShowCrawlerEvent.class);
                eventStack.push(lastEvent.setShowName(showName));
                ShowDialogEvent event = ShowDialogEvent.showOverrideDialog(showName);
                eventStack.push(event);
                userInterface.sendEvent(event);
            } else {
                new SaveShowCrawlerTask(showName, shows, showsCrawler, dateFormat).execute(eventStack, userInterface);
            }
        });
    }
}
