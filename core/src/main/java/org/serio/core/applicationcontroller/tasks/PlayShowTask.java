package org.serio.core.applicationcontroller.tasks;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.event.ShowDialogEvent;
import org.serio.core.applicationcontroller.event.ShowPlayerEvent;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.showplayer.Player;
import org.serio.core.showplayer.ShowPlayer;
import org.serio.core.userinterface.ApplicationEvent;
import org.serio.core.userinterface.UserInterface;

public class PlayShowTask implements ControllerTask {
    private final ShowPlayer showPlayer;

    public PlayShowTask(ShowPlayer showPlayer) {
        this.showPlayer = showPlayer;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(ShowDetailsEvent.class).ifPresent(lastEvent -> {
            DisplayableShow show = lastEvent.getShow();
            Player player = showPlayer.playShow(show.getId(), false);
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
