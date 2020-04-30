package org.serio.core.applicationcontroller.tasks.showdetails;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.applicationcontroller.event.ShowPlayerEvent;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.showplayer.Player;
import org.serio.core.showplayer.ShowPlayer;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.UserInterface;

/**
 * Open the {@link org.serio.core.userinterface.ViewIds#SHOW_PLAYER} view and play the currently selected show
 * from where the user has left it the last time.
 */
public class PlayShowTask implements ControllerTask {
    private final ShowPlayer showPlayer;
    private final boolean fromBeginning;

    /**
     * @see PlayShowTask#PlayShowTask(ShowPlayer, boolean)
     */
    public PlayShowTask(ShowPlayer showPlayer) {
        this(showPlayer, false);
    }

    /**
     * Construct a task.
     *
     * @param showPlayer module that will be used to play the show
     * @param fromBeginning if set to true - the selected will be played starting from the first episode regardless
     *                      of the user's watch history. Otherwise - the show will be played from the moment where
     *                      the user has left it the last time.
     */
    protected PlayShowTask(ShowPlayer showPlayer, boolean fromBeginning) {
        this.showPlayer = showPlayer;
        this.fromBeginning = fromBeginning;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(ShowDetailsEvent.class).ifPresent(lastEvent -> {
            DisplayableShow show = lastEvent.getShow();
            Player player = showPlayer.playShow(show.getId(), fromBeginning);
            ApplicationEvent event;
            if (player.isPlaying()) {
                event = new ShowPlayerEvent(player);
            } else {
                event = ShowDialogEvent.watchIsOverDialog(show.getName());
            }
            eventStack.push(event);
            userInterface.sendEvent(event);
        });
    }
}
