package org.serio.core.applicationcontroller.tasks.showdetails;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.IllegalEventStackStateException;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.applicationcontroller.tasks.ViewAllShowsTask;
import org.serio.core.shows.Shows;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.userinterface.UserInterface;

/**
 * Delete the currently selected show and go back to the {@link org.serio.core.userinterface.ViewIds#ALL_SHOWS} view.
 */
public class DeleteShowTask implements ControllerTask {
    private final Shows shows;
    private final ShowsCrawler showsCrawler;
    private final DateFormat dateFormat;

    /**
     * Construct a task.
     *
     * @param shows module that will be used to get information about the updated list of all shows
     * @param showsCrawler module that will be used to delete the crawler of the show
     * @param dateFormat date format to be applied to show's last watched dates
     */
    public DeleteShowTask(Shows shows, ShowsCrawler showsCrawler, DateFormat dateFormat) {
        this.shows = shows;
        this.showsCrawler = showsCrawler;
        this.dateFormat = dateFormat;
    }

    /**
     * {@inheritDoc}
     */
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
