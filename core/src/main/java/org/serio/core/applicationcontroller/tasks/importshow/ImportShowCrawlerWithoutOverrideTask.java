package org.serio.core.applicationcontroller.tasks.importshow;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ImportShowFromJsonEvent;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.applicationcontroller.model.DateFormat;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.shows.Shows;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.userinterface.UserInterface;

/**
 * Checks if the currently imported show has a name that is already taken.
 *
 * <p>If the imported show has a unique new name - call {@link ImportShowCrawlerTask}.
 * Otherwise - display {@link org.serio.core.userinterface.ViewIds#SHOW_OVERRIDE_DIALOG} and wait
 * for user override confirmation or cancel.</p>
 */
public class ImportShowCrawlerWithoutOverrideTask implements ControllerTask {
    private final Shows shows;
    private final ShowsCrawler showsCrawler;
    private final DateFormat dateFormat;

    /**
     * @see ImportShowCrawlerTask#ImportShowCrawlerTask(Shows, ShowsCrawler, DateFormat)
     */
    public ImportShowCrawlerWithoutOverrideTask(Shows shows, ShowsCrawler showsCrawler, DateFormat dateFormat) {
        this.shows = shows;
        this.showsCrawler = showsCrawler;
        this.dateFormat = dateFormat;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(ImportShowFromJsonEvent.class).ifPresent(lastEvent -> {
            String rawShowCrawler = lastEvent.getShowCrawler().orElse(null);
            String showName = showsCrawler.getShowNameDefinedInShowCrawler(rawShowCrawler);
            if (shows.doesShowWithNameExists(showName)) {
                eventStack.pop(ImportShowFromJsonEvent.class);
                eventStack.push(new ImportShowFromJsonEvent(rawShowCrawler));
                ShowDialogEvent event = ShowDialogEvent.showOverrideDialog(showName);
                eventStack.push(event);
                userInterface.sendEvent(event);
            } else {
                new ImportShowCrawlerTask(shows, showsCrawler, dateFormat).execute(eventStack, userInterface);
            }
        });
    }
}
