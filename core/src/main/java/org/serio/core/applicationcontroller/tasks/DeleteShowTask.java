package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.IllegalEventStackStateException;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.shows.Shows;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.userinterface.UserInterface;

public class DeleteShowTask implements ControllerTask {
    private final Shows shows;
    private final ShowsCrawler showsCrawler;
    private final DateFormat dateFormat;

    public DeleteShowTask(Shows shows, ShowsCrawler showsCrawler, DateFormat dateFormat) {
        this.shows = shows;
        this.showsCrawler = showsCrawler;
        this.dateFormat = dateFormat;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.pop(ShowDialogEvent.class).ifPresent(dialogEvent -> {
            ShowDetailsEvent lastEvent = eventStack.peek(ShowDetailsEvent.class)
                    .orElseThrow(() -> new IllegalEventStackStateException(dialogEvent, ShowDetailsEvent.class, eventStack));
            DisplayableShow show = lastEvent.getShow();
            shows.deleteShow(show.getId());
            showsCrawler.deleteCrawlerOfShow(show.getId().toString());
            eventStack.pop(ShowDetailsEvent.class);
            new ViewAllShowsTask(shows, dateFormat).execute(eventStack, userInterface);
        });
    }
}
