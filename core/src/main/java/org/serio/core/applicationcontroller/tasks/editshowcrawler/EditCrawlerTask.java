package org.serio.core.applicationcontroller.tasks.editshowcrawler;

import org.serio.core.applicationcontroller.event.EditCrawlerEvent;
import org.serio.core.applicationcontroller.event.EditShowCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.userinterface.UserInterface;

/**
 * Opens the view {@link org.serio.core.userinterface.ViewIds#EDIT_CRAWLER} to edit the specified crawler type of
 * the show crawler, currently being edited.
 */
public class EditCrawlerTask implements ControllerTask {
    private final String crawlerType;

    /**
     * Construct a task.
     *
     * @param crawlerType type of crawler to edit
     */
    public EditCrawlerTask(String crawlerType) {
        this.crawlerType = crawlerType;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(EditShowCrawlerEvent.class).ifPresent(lastEvent -> {
            EditCrawlerEvent event = new EditCrawlerEvent(crawlerType, lastEvent.getCrawler(crawlerType).orElse(""));
            eventStack.push(event);
            userInterface.sendEvent(event);
        });
    }
}
