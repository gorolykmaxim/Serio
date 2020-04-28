package org.serio.core.applicationcontroller.tasks.showplayer;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowPlayerEvent;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.showplayer.ShowPlayer;
import org.serio.core.userinterface.UserInterface;

public class ReportShowWatchProgressTask implements ControllerTask {
    private final double progress;
    private final ShowPlayer showPlayer;

    public ReportShowWatchProgressTask(double progress, ShowPlayer showPlayer) {
        this.progress = progress;
        this.showPlayer = showPlayer;
    }

    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        if (eventStack.isLastEventOfType(ShowPlayerEvent.class)) {
            showPlayer.reportPlaybackProgress(progress);
        }
    }
}
