package org.serio.core.applicationcontroller.tasks.showplayer;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowPlayerEvent;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.showplayer.ShowPlayer;
import org.serio.core.userinterface.UserInterface;

/**
 * Report the playback progress of the currently playing show episode in percentages.
 *
 * <p>If report attempt fails for some reason - no exception will be thrown by this task.</p>
 */
public class ReportShowWatchProgressTask implements ControllerTask {
    private final double progress;
    private final ShowPlayer showPlayer;

    /**
     * Construct a task.
     *
     * @param progress current playback progress
     * @param showPlayer module that is used to play the show
     */
    public ReportShowWatchProgressTask(double progress, ShowPlayer showPlayer) {
        this.progress = progress;
        this.showPlayer = showPlayer;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        if (eventStack.isLastEventOfType(ShowPlayerEvent.class)) {
            try {
                showPlayer.reportPlaybackProgress(progress);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
