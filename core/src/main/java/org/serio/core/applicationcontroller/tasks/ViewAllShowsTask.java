package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.AllShowsEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.shows.Shows;
import org.serio.core.userinterface.UserInterface;

public class ViewAllShowsTask implements ControllerTask {
    private final Shows shows;
    private final DateFormat dateFormat;

    public ViewAllShowsTask(Shows shows, DateFormat dateFormat) {
        this.shows = shows;
        this.dateFormat = dateFormat;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        AllShowsEvent event = new AllShowsEvent(shows.findAllShows(), dateFormat);
        eventStack.clear();
        eventStack.push(event);
        userInterface.sendEvent(event);
    }
}
