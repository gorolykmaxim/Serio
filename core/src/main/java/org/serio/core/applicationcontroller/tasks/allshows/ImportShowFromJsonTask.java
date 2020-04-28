package org.serio.core.applicationcontroller.tasks.allshows;

import org.serio.core.applicationcontroller.event.AllShowsEvent;
import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ImportShowFromJsonEvent;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.userinterface.UserInterface;

public class ImportShowFromJsonTask implements ControllerTask {
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        if (eventStack.isLastEventOfType(AllShowsEvent.class)) {
            ImportShowFromJsonEvent event = new ImportShowFromJsonEvent();
            eventStack.push(event);
            userInterface.sendEvent(event);
        }
    }
}
