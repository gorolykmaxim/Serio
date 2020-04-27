package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.AllShowsEvent;
import org.serio.core.applicationcontroller.event.EditShowCrawlerEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.userinterface.UserInterface;

public class AddShowTask implements ControllerTask {
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        if (eventStack.isLastEventOfType(AllShowsEvent.class)) {
            EditShowCrawlerEvent event = new EditShowCrawlerEvent();
            eventStack.push(event);
            userInterface.sendEvent(event);
        }
    }
}
