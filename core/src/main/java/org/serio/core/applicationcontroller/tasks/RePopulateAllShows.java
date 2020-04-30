package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.AllShowsEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.shows.Shows;
import org.serio.core.shows.WatchableShowList;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.UserInterface;
import org.serio.core.userinterface.ViewIds;

import java.util.List;

public class RePopulateAllShows implements ControllerTask {
    private final Shows shows;
    private final DateFormat dateFormat;

    public RePopulateAllShows(Shows shows, DateFormat dateFormat) {
        this.shows = shows;
        this.dateFormat = dateFormat;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        try {
            WatchableShowList allShows = shows.findAllShows();
            List<ApplicationEvent> events = eventStack.toList();
            events.clear();
            eventStack.push(new AllShowsEvent(allShows, dateFormat));
            events.stream().filter(event -> ViewIds.ALL_SHOWS != event.getViewId()).forEach(eventStack::push);
        } catch (Exception e) {
            // In some cases propagating this exception can lead to bad things:
            // At this point the crawled show is saved, so the user will be able see it.
            // Though for some reason we were not able to re-query all shows list to re-build the AllShowsEvent
            // at the root of the stack to incorporate the new show.
            // If we throw this error not only the user will not be directed to the new show's details view,
            // but the user will remain in the currently open show add / edit / import view. Moreover
            // if the user will go back to the all shows view - the newly crawled show will be missing, making
            // the user think that the show has not been crawled at all, which is not the case.
            // From all the bad options we have - just swallow this exception and proceed with the usual routine:
            // try to display show details view for the crawled show: it might not fail and the user will see the show.
            // The bad thing is: the AllShowsEvent at the bottom of the stack will not get updated, but it's
            // the lesser of all the evils in this situation.
            e.printStackTrace();
        }
    }
}
