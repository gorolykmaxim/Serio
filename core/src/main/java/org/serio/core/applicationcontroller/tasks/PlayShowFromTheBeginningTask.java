package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.showplayer.ShowPlayer;
import org.serio.core.userinterface.UserInterface;

public class PlayShowFromTheBeginningTask extends PlayShowTask implements ControllerTask {
    public PlayShowFromTheBeginningTask(ShowPlayer showPlayer) {
        super(showPlayer, true);
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        if (eventStack.pop(ShowDialogEvent.class).isPresent()) {
            super.execute(eventStack, userInterface);
        }
    }
}
