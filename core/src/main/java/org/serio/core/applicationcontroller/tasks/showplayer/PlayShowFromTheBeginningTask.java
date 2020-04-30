package org.serio.core.applicationcontroller.tasks.showplayer;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.applicationcontroller.tasks.showdetails.PlayShowTask;
import org.serio.core.showplayer.ShowPlayer;
import org.serio.core.userinterface.UserInterface;

/**
 * Re-open the {@link org.serio.core.userinterface.ViewIds#SHOW_PLAYER} view and play the previously played show
 * from the first episode.
 */
public class PlayShowFromTheBeginningTask extends PlayShowTask implements ControllerTask {
    /**
     * Construct a task.
     *
     * @param showPlayer module that is used to play the show
     */
    public PlayShowFromTheBeginningTask(ShowPlayer showPlayer) {
        super(showPlayer, true);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        if (eventStack.pop(ShowDialogEvent.class).isPresent()) {
            super.execute(eventStack, userInterface);
        }
    }
}
