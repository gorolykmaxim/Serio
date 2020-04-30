package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.AllShowsEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.shows.Shows;
import org.serio.core.userinterface.UserInterface;

/**
 * The task that initializes the initial state of {@link org.serio.core.applicationcontroller.ApplicationController}.
 *
 * <p>Sends {@link AllShowsEvent} to the user interface and places it at the bottom of the stack, while clearing
 * the latter one beforehand. When this task get's executed, it completely resets the UI to the ZERO state.</p>
 */
public class ViewAllShowsTask implements ControllerTask {
    private final Shows shows;
    private final DateFormat dateFormat;

    /**
     * Construct a task.
     *
     * @param shows module that will be used to get list of shows to be displayed
     * @param dateFormat date format to be applied to show's last watched dates
     */
    public ViewAllShowsTask(Shows shows, DateFormat dateFormat) {
        this.shows = shows;
        this.dateFormat = dateFormat;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        AllShowsEvent event = new AllShowsEvent(shows.findAllShows(), dateFormat);
        eventStack.clear();
        eventStack.push(event);
        userInterface.sendEvent(event);
    }
}
