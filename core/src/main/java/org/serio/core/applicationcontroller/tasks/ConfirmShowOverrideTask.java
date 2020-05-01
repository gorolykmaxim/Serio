package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.EditShowCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ImportShowFromJsonEvent;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.tasks.editshowcrawler.SaveShowCrawlerTask;
import org.serio.core.applicationcontroller.tasks.importshow.ImportShowCrawlerTask;
import org.serio.core.shows.Shows;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.userinterface.UserInterface;

/**
 * The task that confirms the override of an existing show with a new show with the same name.
 *
 * <p>Crawls and save the new show, while replacing the existing one, and displays
 * the {@link org.serio.core.userinterface.ViewIds#SHOW_DETAILS} view of the new show.</p>
 */
public class ConfirmShowOverrideTask implements ControllerTask {
    private final Shows shows;
    private final ShowsCrawler showsCrawler;
    private final DateFormat dateFormat;

    /**
     * Construct a task.
     *
     * @param shows module that will be used to get list of shows to be displayed
     * @param showsCrawler module that will be used to crawl the new show
     * @param dateFormat date format to be applied to show's last watched dates
     */
    public ConfirmShowOverrideTask(Shows shows, ShowsCrawler showsCrawler, DateFormat dateFormat) {
        this.shows = shows;
        this.showsCrawler = showsCrawler;
        this.dateFormat = dateFormat;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        if (eventStack.pop(ShowDialogEvent.class).isPresent()) {
            if (eventStack.isLastEventOfType(ImportShowFromJsonEvent.class)) {
                new ImportShowCrawlerTask(shows, showsCrawler, dateFormat).execute(eventStack, userInterface);
            } else if (eventStack.isLastEventOfType(EditShowCrawlerEvent.class)) {
                new SaveShowCrawlerTask(shows, showsCrawler, dateFormat).execute(eventStack, userInterface);
            }
        }
    }
}
