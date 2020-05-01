package org.serio.core.applicationcontroller.tasks.editshowcrawler;

import org.serio.core.applicationcontroller.event.EditShowCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.shows.Shows;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.userinterface.UserInterface;

/**
 * Checks if the currently edited show is new and has a name of a show, that already exists.
 *
 * <p>If the currently edited show is not new or has a unique new name - call {@link SaveShowCrawlerTask}.
 * Otherwise - display {@link org.serio.core.userinterface.ViewIds#SHOW_OVERRIDE_DIALOG} and wait
 * for user override confirmation or cancel.</p>
 */
public class SaveShowCrawlerWithoutOverrideTask implements ControllerTask {
    private final Shows shows;
    private final ShowsCrawler showsCrawler;
    private final DateFormat dateFormat;

    /**
     * @see SaveShowCrawlerTask#SaveShowCrawlerTask(Shows, ShowsCrawler, DateFormat)
     */
    public SaveShowCrawlerWithoutOverrideTask(Shows shows, ShowsCrawler showsCrawler, DateFormat dateFormat) {
        this.shows = shows;
        this.showsCrawler = showsCrawler;
        this.dateFormat = dateFormat;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(EditShowCrawlerEvent.class).ifPresent(lastEvent -> {
            String showName = lastEvent.getShowName().orElse(null);
            if (!lastEvent.isEditingExistingShow() && shows.doesShowWithNameExists(showName)) {
                eventStack.pop(EditShowCrawlerEvent.class);
                eventStack.push(lastEvent.setShowName(showName));
                ShowDialogEvent event = ShowDialogEvent.showOverrideDialog(showName);
                eventStack.push(event);
                userInterface.sendEvent(event);
            } else {
                new SaveShowCrawlerTask(shows, showsCrawler, dateFormat).execute(eventStack, userInterface);
            }
        });
    }
}
