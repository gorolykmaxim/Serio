package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.event.ShowPlayerEvent;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.showplayer.Player;
import org.serio.core.showplayer.ShowPlayer;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.UserInterface;

public class PlayShowEpisodeTask implements ControllerTask {
    private final int episodeNumber;
    private final ShowPlayer showPlayer;

    public PlayShowEpisodeTask(int episodeNumber, ShowPlayer showPlayer) {
        this.episodeNumber = episodeNumber;
        this.showPlayer = showPlayer;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(ShowDetailsEvent.class).ifPresent(lastEvent -> {
            DisplayableShow show = lastEvent.getShow();
            Player player = showPlayer.playShowEpisode(show.getId(), episodeNumber);
            if (player.isPlaying()) {
                ApplicationEvent event = new ShowPlayerEvent(player);
                eventStack.push(event);
                userInterface.sendEvent(event);
            } else if (!show.getEpisodeById(episodeNumber).isPresent()) {
                throw new IllegalArgumentException(String.format("%s does not have an episode with ID %d", show, episodeNumber));
            }
        });
    }
}
