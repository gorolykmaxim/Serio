package org.serio.core.applicationcontroller.tasks.showdetails;

import org.serio.core.applicationcontroller.event.EventStack;
import org.serio.core.applicationcontroller.event.ShowDetailsEvent;
import org.serio.core.applicationcontroller.model.DisplayableShow;
import org.serio.core.applicationcontroller.tasks.ControllerTask;
import org.serio.core.clipboard.Clipboard;
import org.serio.core.notifications.Notifications;
import org.serio.core.showscrawler.ShowsCrawler;
import org.serio.core.userinterface.UserInterface;

/**
 * Copy the crawler of the currently selected show to the clipboard and display a notification to the user about it.
 */
public class ShareShowCrawlerTask implements ControllerTask {
    private final Notifications notifications;
    private final Clipboard clipboard;
    private final ShowsCrawler showsCrawler;

    /**
     * Construct a task.
     *
     * @param notifications module that will be used to display notifications
     * @param clipboard module that will be used to save the show crawler to the clipboard
     * @param showsCrawler module that will be used obtain crawler configuration of the show
     */
    public ShareShowCrawlerTask(Notifications notifications, Clipboard clipboard, ShowsCrawler showsCrawler) {
        this.notifications = notifications;
        this.clipboard = clipboard;
        this.showsCrawler = showsCrawler;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void execute(EventStack eventStack, UserInterface userInterface) {
        eventStack.peek(ShowDetailsEvent.class).ifPresent(lastEvent -> {
            DisplayableShow show = lastEvent.getShow();
            String serializedShowCrawler = showsCrawler.getSerializedCrawlerOfShow(show.getId().toString());
            clipboard.setContent(serializedShowCrawler);
            notifications.displayTextAsNotification("Crawler is copied to your clipboard");
        });
    }
}
