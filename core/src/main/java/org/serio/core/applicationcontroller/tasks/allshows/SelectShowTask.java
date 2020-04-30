package org.serio.core.applicationcontroller.tasks.allshows;

import org.serio.core.applicationcontroller.event.AllShowsEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.shows.Shows;
import org.serio.core.userinterface.UserInterface;

import java.util.UUID;

/**
 * Opens the {@link org.serio.core.userinterface.ViewIds#SHOW_DETAILS} view to display the specified show.
 */
public class SelectShowTask implements ControllerTask {
    private final String showId;
    private final Shows shows;
    private final DateFormat dateFormat;

    /**
     * Construct a task.
     *
     * @param showId ID of the show, that show be displayed in the view
     * @param shows module that will be used to get list of shows to be displayed
     * @param dateFormat date format to be applied to show's last watched dates
     */
    public SelectShowTask(String showId, Shows shows, DateFormat dateFormat) {
        this.showId = showId;
        this.shows = shows;
        this.dateFormat = dateFormat;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        if (eventStack.isLastEventOfType(AllShowsEvent.class)) {
            UUID id = UUID.fromString(showId);
            DisplayableShow show = new DisplayableShow(shows.findShowById(id), dateFormat);
            ShowDetailsEvent event = new ShowDetailsEvent(show);
            eventStack.push(event);
            userInterface.sendEvent(event);
        }
    }
}
