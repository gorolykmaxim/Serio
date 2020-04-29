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

import java.util.Optional;

public class ConfirmShowOverrideTask implements ControllerTask {
    private final Shows shows;
    private final ShowsCrawler showsCrawler;
    private final DateFormat dateFormat;

    public ConfirmShowOverrideTask(Shows shows, ShowsCrawler showsCrawler, DateFormat dateFormat) {
        this.shows = shows;
        this.showsCrawler = showsCrawler;
        this.dateFormat = dateFormat;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        if (eventStack.pop(ShowDialogEvent.class).isPresent()) {
            Optional<ImportShowFromJsonEvent> importEvent = eventStack.peek(ImportShowFromJsonEvent.class);
            Optional<EditShowCrawlerEvent> editEvent = eventStack.peek(EditShowCrawlerEvent.class);
            if (importEvent.isPresent()) {
                ImportShowFromJsonEvent event = importEvent.get();
                new ImportShowCrawlerTask(event.getRawShowCrawler().orElse(null), shows, showsCrawler, dateFormat).execute(eventStack, userInterface);
            } else if (editEvent.isPresent()) {
                EditShowCrawlerEvent event = editEvent.get();
                new SaveShowCrawlerTask(event.getShowName().orElse(null), shows, showsCrawler, dateFormat).execute(eventStack, userInterface);
            }
        }
    }
}
