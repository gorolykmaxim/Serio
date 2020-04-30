package org.serio.core.applicationcontroller.tasks.showplayer;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowPlayerEvent;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.showplayer.Player;
import org.serio.core.showplayer.ShowPlayer;
import org.serio.core.userinterface.UserInterface;

/**
 * Update the currently rendered {@link org.serio.core.userinterface.ViewIds#SHOW_PLAYER} to play the previous episode
 * of the currently playing show.
 *
 * <p>If the currently playing episode is the first one - this call will be ignored.</p>
 */
public class PlayPreviousEpisodeTask implements ControllerTask {
    private final ShowPlayer showPlayer;

    /**
     * Construct a task.
     *
     * @param showPlayer module that is used to play the show
     */
    public PlayPreviousEpisodeTask(ShowPlayer showPlayer) {
        this.showPlayer = showPlayer;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        if (eventStack.isLastEventOfType(ShowPlayerEvent.class)) {
            Player player = showPlayer.playPreviousEpisode();
            if (player.isPlaying()) {
                ShowPlayerEvent event = new ShowPlayerEvent(player);
                eventStack.pop(ShowPlayerEvent.class);
                eventStack.push(event);
                userInterface.sendEvent(event);
            }
        }
    }
}
