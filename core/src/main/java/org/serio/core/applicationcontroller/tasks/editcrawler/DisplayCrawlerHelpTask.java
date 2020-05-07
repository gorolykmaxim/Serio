package org.serio.core.applicationcontroller.tasks.editcrawler;

import org.serio.core.applicationcontroller.event.CrawlerHelpEvent;
import org.serio.core.applicationcontroller.event.EditCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.userinterface.UserInterface;

/**
 * Opens the {@link org.serio.core.userinterface.ViewIds#CRAWLER_HELP} view.
 */
public class DisplayCrawlerHelpTask implements ControllerTask {
    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        if (eventStack.isLastEventOfType(EditCrawlerEvent.class)) {
            CrawlerHelpEvent event = new CrawlerHelpEvent();
            eventStack.push(event);
            userInterface.sendEvent(event);
        }
    }
}
