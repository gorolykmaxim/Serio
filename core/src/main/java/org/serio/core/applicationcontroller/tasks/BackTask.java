package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.tasks.allshows.SelectShowTask;
import org.serio.core.shows.Shows;
import org.serio.core.userinterface.UserInterface;
import org.serio.core.userinterface.ViewIds;

/**
 * Handles the "back" user interface operation where the currently displayed view should be discarded an the previously
 * displayed view should be displayed once again.
 *
 * <p>Simply pops the current head of the event stack and sends the new head of the stack to the UI.</p>
 *
 * <p>IF the new top event is the {@link org.serio.core.applicationcontroller.event.AllShowsEvent} - call
 * the {@link ViewAllShowsTask} to replace it with an updated version that will have an updated list
 * of all shows with an up-to-date information.</p>
 */
public class BackTask implements ControllerTask {
    private final Shows shows;
    private final DateFormat dateFormat;

    /**
     * Construct a task.
     *
     * @param shows module that will be used to get updated list of all shows, in the task would need to display
     *              the {@link ViewIds#ALL_SHOWS} view
     * @param dateFormat format that will be applied to show's last watched dates
     */
    public BackTask(Shows shows, DateFormat dateFormat) {
        this.shows = shows;
        this.dateFormat = dateFormat;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.popAndPeek().ifPresent(newTopEvent -> {
            // Crutch of this ApplicationController's implementation.
            if (ViewIds.ALL_SHOWS == newTopEvent.getViewId()) {
                // If the new top event is AllShowsEvent - execute ViewAllShowsTask
                // to replace the existing event with a new one with the updated list of displayed shows.
                new ViewAllShowsTask(shows, dateFormat).execute(eventStack, userInterface);
            } else if (ViewIds.SHOW_DETAILS == newTopEvent.getViewId()) {
                // If the new top event is ShowDetailsEvent - execute SelectShowTask
                // to replace the existing evnet with a new one with the updated list of watched episodes.
                eventStack.pop(ShowDetailsEvent.class).ifPresent(event -> {
                    new SelectShowTask(event.getShow().getId().toString(), shows, dateFormat).execute(eventStack, userInterface);
                });
            } else {
                userInterface.sendEvent(newTopEvent);
            }
        });
    }
}
