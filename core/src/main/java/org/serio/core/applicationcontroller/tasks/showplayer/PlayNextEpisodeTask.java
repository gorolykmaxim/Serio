package org.serio.core.applicationcontroller.tasks.showplayer;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.applicationcontroller.event.ShowPlayerEvent;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.showplayer.Player;
import org.serio.core.showplayer.ShowPlayer;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.UserInterface;

/**
 * Update the currently rendered {@link org.serio.core.userinterface.ViewIds#SHOW_PLAYER} to play the next episode
 * of the currently playing show.
 *
 * <p>If the currently playing episode is the last one - display
 * the {@link org.serio.core.userinterface.ViewIds#SHOW_WATCH_IS_OVER_DIALOG} view.</p>
 */
public class PlayNextEpisodeTask implements ControllerTask {
    private final ShowPlayer showPlayer;

    /**
     * Construct a task.
     *
     * @param showPlayer module that is used to play the show
     */
    public PlayNextEpisodeTask(ShowPlayer showPlayer) {
        this.showPlayer = showPlayer;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        if (eventStack.isLastEventOfType(ShowPlayerEvent.class)) {
            Player player = showPlayer.playNextEpisode();
            ApplicationEvent event;
            if (player.isPlaying()) {
                event = new ShowPlayerEvent(player);
            } else {
                event = ShowDialogEvent.watchIsOverDialog(player.getPlayingShow().getName());
            }
            eventStack.pop(ShowPlayerEvent.class);
            eventStack.push(event);
            userInterface.sendEvent(event);
        }
    }
}
