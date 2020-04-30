package org.serio.core.applicationcontroller.tasks.showdetails;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.IllegalEventStackStateException;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.applicationcontroller.tasks.RePopulateAllShows;
import org.serio.core.applicationcontroller.tasks.allshows.SelectShowTask;
import org.serio.core.shows.Shows;
import org.serio.core.userinterface.UserInterface;

/**
 * Clear the user's watch history of the currently selected show, and update the displayed
 * {@link org.serio.core.userinterface.ViewIds#SHOW_DETAILS} view.
 */
public class ClearWatchHistoryTask implements ControllerTask {
    private final Shows shows;
    private final DateFormat dateFormat;

    /**
     * Construct a task.
     *
     * @param shows module that will be used to clear watch history and get updated information about the selected show
     * @param dateFormat date format to be applied to show's last watched dates
     */
    public ClearWatchHistoryTask(Shows shows, DateFormat dateFormat) {
        this.shows = shows;
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
            shows.clearWatchHistoryOfShow(show.getId());
            new RePopulateAllShows(shows, dateFormat).execute(eventStack, userInterface);
            eventStack.pop(ShowDetailsEvent.class);
            new SelectShowTask(show.getId().toString(), shows, dateFormat).execute(eventStack, userInterface);
        });
    }
}
