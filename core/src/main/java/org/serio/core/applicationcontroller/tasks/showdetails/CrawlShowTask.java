package org.serio.core.applicationcontroller.tasks.showdetails;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.applicationcontroller.tasks.allshows.SelectShowTask;
import org.serio.core.shows.Shows;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.showstorage.Show;
import org.serio.core.userinterface.UserInterface;

public class CrawlShowTask implements ControllerTask {
    private final ShowsCrawler showsCrawler;
    private final Shows shows;
    private final DateFormat dateFormat;

    public CrawlShowTask(ShowsCrawler showsCrawler, Shows shows, DateFormat dateFormat) {
        this.showsCrawler = showsCrawler;
        this.shows = shows;
        this.dateFormat = dateFormat;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(ShowDetailsEvent.class).ifPresent(lastEvent -> {
            DisplayableShow show = lastEvent.getShow();
            Show updatedShow = showsCrawler.crawlShow(show.getId().toString());
            shows.saveShow(updatedShow);
            eventStack.pop(ShowDetailsEvent.class);
            new SelectShowTask(updatedShow.getId().toString(), shows, dateFormat).execute(eventStack, userInterface);
        });
    }
}
