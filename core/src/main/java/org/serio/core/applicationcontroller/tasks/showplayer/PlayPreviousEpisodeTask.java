package org.serio.core.applicationcontroller.tasks.showplayer;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowPlayerEvent;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.showplayer.Player;
import org.serio.core.showplayer.ShowPlayer;
import org.serio.core.userinterface.UserInterface;

public class PlayPreviousEpisodeTask implements ControllerTask {
    private final ShowPlayer showPlayer;

    public PlayPreviousEpisodeTask(ShowPlayer showPlayer) {
        this.showPlayer = showPlayer;
    }

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
